namespace Mustard::Math::Random::inline Distribution {

namespace internal {

template<std::floating_point T, template<typename> typename AGaussian>
constexpr BasicGaussianParameter<T, AGaussian>::BasicGaussianParameter(T mu, T sigma) :
    Base{},
    fMu{mu},
    fSigma{sigma} {}

template<std::floating_point T, template<typename> typename AGaussian>
template<Concept::Character AChar>
auto BasicGaussianParameter<T, AGaussian>::StreamOutput(std::basic_ostream<AChar>& os) const -> decltype(os) {
    const auto oldPrecision{os.precision(std::numeric_limits<T>::max_digits10)};
    return os << fMu << ' ' << fSigma
              << std::setprecision(oldPrecision);
}

template<std::floating_point T, template<typename> typename AGaussian>
template<Concept::Character AChar>
auto BasicGaussianParameter<T, AGaussian>::StreamInput(std::basic_istream<AChar>& is) & -> decltype(is) {
    return is >> fMu >> fSigma;
}

template<std::floating_point T, template<typename> typename AGaussian>
constexpr GaussianBase<T, AGaussian>::GaussianBase(T mu, T sigma) :
    Base{},
    fParameter{mu, sigma} {}

template<std::floating_point T, template<typename> typename AGaussian>
constexpr GaussianBase<T, AGaussian>::GaussianBase(const typename Base::ParameterType& p) :
    Base{},
    fParameter{p} {}

} // namespace internal

#define MUSTARD_MATH_RANDOM_DISTRIBUTION_GAUSSIAN_GENERATOR_SNIPPET(Suffix)       \
    if ((fSaved = not fSaved)) {                                                  \
        static_assert(Gaussian2DDiagnoal##Suffix<std::array<T, 2>>::Stateless()); \
        auto&& [u, v]{Gaussian2DDiagnoal##Suffix<std::array<T, 2>>{}(g)};         \
        fSavedValue = v;                                                          \
        return p.Sigma() * u + p.Mu();                                            \
    } else {                                                                      \
        return p.Sigma() * fSavedValue + p.Mu();                                  \
    }

template<std::floating_point T>
MUSTARD_STRONG_INLINE auto Gaussian<T>::operator()(UniformRandomBitGenerator auto& g, const GaussianParameter<T>& p) -> T {
    MUSTARD_MATH_RANDOM_DISTRIBUTION_GAUSSIAN_GENERATOR_SNIPPET()
}

template<std::floating_point T>
MUSTARD_STRONG_INLINE auto GaussianFast<T>::operator()(UniformRandomBitGenerator auto& g, const GaussianFastParameter<T>& p) -> T {
    MUSTARD_MATH_RANDOM_DISTRIBUTION_GAUSSIAN_GENERATOR_SNIPPET(Fast)
}

#undef MUSTARD_MATH_RANDOM_DISTRIBUTION_GAUSSIAN_GENERATOR_SNIPPET

} // namespace Mustard::Math::Random::inline Distribution
