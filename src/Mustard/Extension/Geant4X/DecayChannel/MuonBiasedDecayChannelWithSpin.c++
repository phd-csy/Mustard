// -*- C++ -*-
//
// Copyright 2020-2024  The Mustard development team
//
// This file is part of Mustard, an offline software framework for HEP experiments.
//
// Mustard is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// Mustard is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// Mustard. If not, see <https://www.gnu.org/licenses/>.

#include "Mustard/Extension/Geant4X/DecayChannel/MuonBiasedDecayChannelWithSpin.h++"
#include "Mustard/Utility/MathConstant.h++"
#include "Mustard/Utility/PhysicalConstant.h++"

#include "G4DecayProducts.hh"
#include "G4MuonDecayChannelWithSpin.hh"
#include "G4SPSRandomGenerator.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "Randomize.hh"

#include "Eigen/Core"
#include "Eigen/Geometry"

#include <memory>

namespace Mustard::inline Extension::Geant4X::inline DecayChannel {

using namespace PhysicalConstant;
using namespace MathConstant;

MuonBiasedDecayChannelWithSpin::MuonBiasedDecayChannelWithSpin(const G4String& parentName, G4double br) :
    G4MuonDecayChannelWithSpin{parentName, br},
    fEnergyCut{0.},
    fMinTheta{0.},
    fMaxTheta{0.},
    fMessengerRegister{this} {}

auto GenerateConicFlux(G4double MinTheta, G4double MaxTheta, G4ThreeVector& mom) -> auto {
    const auto cosTheta{std::cos(MinTheta / 2) - G4UniformRand() * (std::cos(MinTheta / 2) - std::cos(MaxTheta / 2))};
    const auto sinTheta{std::sqrt(1. - cosTheta * cosTheta)};
    const auto phi{2 * pi * G4UniformRand()};

    const Eigen::Vector3d inputMomentum{sinTheta * std::cos(phi), sinTheta * std::sin(phi), cosTheta};
    const auto rotMatrix = Eigen::Quaterniond::FromTwoVectors(Eigen::Vector3d{0, 0, 1}, Eigen::Vector3d{mom.x(), mom.y(), mom.z()}).toRotationMatrix();
    const auto outputMomentum{rotMatrix * inputMomentum};

    return G4ThreeVector{outputMomentum.x(), outputMomentum.y(), outputMomentum.z()};
}

auto MuonBiasedDecayChannelWithSpin::DecayIt(G4double) -> G4DecayProducts* {
    while (true) {
        const auto products{G4MuonDecayChannelWithSpin::DecayIt(muon_mass_c2)};
        const auto positron{(*products)[0]};
        auto initialDirection{positron->GetMomentumDirection()};

        if (positron->GetKineticEnergy() > fEnergyCut) {
            const auto pileupProducts{G4MuonDecayChannelWithSpin::DecayIt(muon_mass_c2)};
            const auto pileupPositron{(*pileupProducts)[0]};
            auto pileupDirection{-initialDirection};

            if (pileupPositron->GetKineticEnergy() > fEnergyCut) {
                pileupPositron->SetMomentumDirection(GenerateConicFlux(fMinTheta, fMaxTheta, pileupDirection));
                products->PushProducts(pileupPositron);
                return products;
            }
        }
        delete products;
    }
}

} // namespace Mustard::inline Extension::Geant4X::inline DecayChannel
