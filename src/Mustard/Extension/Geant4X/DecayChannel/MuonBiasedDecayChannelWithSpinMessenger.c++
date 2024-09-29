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
#include "Mustard/Extension/Geant4X/DecayChannel/MuonBiasedDecayChannelWithSpinMessenger.h++"

#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIdirectory.hh"

namespace Mustard::inline Extension::Geant4X::inline DecayChannel {

MuonBiasedDecayChannelWithSpinMessenger::MuonBiasedDecayChannelWithSpinMessenger() :
    SingletonMessenger{},
    fDirectory{},
    fEnergyCut{},
    fMinTheta{},
    fMaxTheta{} {

    fDirectory = std::make_unique<G4UIdirectory>("/Mustard/Physics/MuonDecay/Biasing/");
    fDirectory->SetGuidance("Muon(ium) decay channel biasing.");

    fEnergyCut = std::make_unique<G4UIcmdWithADoubleAndUnit>("/Mustard/Physics/MuonDecay/Biasing/EnergyCut", this);
    fEnergyCut->SetGuidance("Set energy cut for muon(ium) decay channel.");
    fEnergyCut->SetParameterName("E", false);
    fEnergyCut->SetDefaultValue(0.);
    fEnergyCut->SetDefaultUnit("MeV");
    fEnergyCut->SetRange("E >= 0");
    fEnergyCut->AvailableForStates(G4State_PreInit, G4State_Idle);

    fMinTheta = std::make_unique<G4UIcmdWithADoubleAndUnit>("/Mustard/Physics/MuonDecay/Biasing/MinTheta", this);
    fMinTheta->SetGuidance("Set emission angle biasing for muon(ium) decay channel.");
    fMinTheta->SetParameterName("theta", false);
    fMinTheta->SetDefaultValue(0.);
    fMinTheta->SetDefaultUnit("deg");
    fMinTheta->SetRange("theta >= 0");
    fMinTheta->AvailableForStates(G4State_PreInit, G4State_Idle);

    fMaxTheta = std::make_unique<G4UIcmdWithADoubleAndUnit>("/Mustard/Physics/MuonDecay/Biasing/MaxTheta", this);
    fMaxTheta->SetGuidance("Set emission angle biasing for muon(ium) decay channel.");
    fMaxTheta->SetParameterName("theta", false);
    fMaxTheta->SetDefaultValue(180.);
    fMaxTheta->SetDefaultUnit("deg");
    fMaxTheta->SetRange("theta <= 180");
    fMaxTheta->AvailableForStates(G4State_PreInit, G4State_Idle);
}

MuonBiasedDecayChannelWithSpinMessenger::~MuonBiasedDecayChannelWithSpinMessenger() = default;

auto MuonBiasedDecayChannelWithSpinMessenger::SetNewValue(G4UIcommand* command, G4String value) -> void {
    if (command == fEnergyCut.get()) {
        Deliver<MuonBiasedDecayChannelWithSpin>([&](auto&& r) {
            r.EnergyCut(fEnergyCut->GetNewDoubleValue(value));
        });
    } else if (command == fMinTheta.get()) {
        Deliver<MuonBiasedDecayChannelWithSpin>([&](auto&& r) {
            r.MinTheta(fMinTheta->GetNewDoubleValue(value));
        });
    } else if (command == fMaxTheta.get()) {
        Deliver<MuonBiasedDecayChannelWithSpin>([&](auto&& r) {
            r.MaxTheta(fMaxTheta->GetNewDoubleValue(value));
        });
    }
}

} // namespace Mustard::inline Extension::Geant4X::inline DecayChannel
