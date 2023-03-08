#include "csignal.h"

template <>
csignal<double>::operator csignal<unsigned char>()
{
    csignal<unsigned char> temp(this->size());
    temp.setServiceParameters(this->m_azimuth, this->m_frameNumber);
    std::transform(std::begin(this->m_signal), std::end(this->m_signal), temp.begin(),
                   [](double i)
    {
        if(i < 0)
            return 0;
        if(i > 2047)
            return 255;
        return (static_cast<short>(i) >> 3);
    });
    return temp;
}

//template <>
//csignal<unsigned char>::operator csignal<double>()
//{
//    csignal<double> temp(this->size());
//    temp.setServiceParameters(this->m_azimuth, this->m_frameNumber);
//    std::transform(std::begin(this->m_signal), std::end(this->m_signal), temp.begin(),
//                   [](unsigned char i) { return (static_cast<double>(i << 3)); });
//    return temp;
//}

template <>
csignal<unsigned char>& csignal<unsigned char>::operator += (const int val)
{
    std::transform(begin(), end(), begin(), [val]( unsigned char& sigVal)
    {
        int temp = val + sigVal;

        if( temp < std::numeric_limits<unsigned char>::min())
            return std::numeric_limits<unsigned char>::min();
        if( temp > std::numeric_limits<unsigned char>::max())
            return std::numeric_limits<unsigned char>::max();

        return  static_cast<unsigned char>(temp);
    });
    return *this;
}

template <>
csignal<unsigned char>& csignal<unsigned char>::operator *= (const int mult)
{
    std::transform(begin(), end(), begin(), [mult]( unsigned char& sigVal)
    {
        int temp = mult * sigVal;
        if( temp < std::numeric_limits<unsigned char>::min())
            return std::numeric_limits<unsigned char>::min();
        if( temp > std::numeric_limits<unsigned char>::max())
            return std::numeric_limits<unsigned char>::max();
        return  static_cast<unsigned char>(temp);
    });
    return *this;
}

csignal<double> Sqrt(const csignal<double>& sig)
{
    csignal<double> temp(sig.size());
    temp.setServiceParameters(sig.azimuth(), sig.frameNumber());
    std::transform(sig.begin(), sig.end(), temp.begin(), sqrt);
    return temp;
}

csignal<double> Atan(const csignal<double>& sig)
{    
    csignal<double> temp(sig.size());
    temp.setServiceParameters(sig.azimuth(), sig.frameNumber());
    std::transform(sig.begin(), sig.end(), temp.begin(), atan);
    return temp;
}

csignal<double> Tan(const csignal<double>& sig)
{
    csignal<double> temp(sig.size());
    temp.setServiceParameters(sig.azimuth(), sig.frameNumber());
    std::transform(sig.begin(), sig.end(), temp.begin(), tan);
    return temp;
}

csignal<double> Cos(const csignal<double>& sig)
{
    csignal<double> temp(sig.size());
    temp.setServiceParameters(sig.azimuth(), sig.frameNumber());
    std::transform(sig.begin(), sig.end(), temp.begin(), cos);
    return temp;
}

csignal<double> Sin(const csignal<double>& sig)
{
    csignal<double> temp(sig.size());
    temp.setServiceParameters(sig.azimuth(), sig.frameNumber());
    std::transform(sig.begin(), sig.end(), temp.begin(), sin);
    return temp;
}

csignal<double> AbsComplex(const csignal<double> &reSig, const csignal<double> &imSig)
{
    if(reSig.size() == imSig.size())
    {
        csignal<double> temp(reSig.size());
        temp.setServiceParameters(reSig.azimuth(), reSig.frameNumber());
        std::transform(reSig.begin(), reSig.end(), imSig.begin(), temp.begin(),
                       [](double reVal, double imVal){ return std::abs(std::complex<double>(reVal, imVal)); });
        return temp;
    }

    throw csignal<double>::CsignalLengthMismatch();
}

csignal<double> ArgComplex(const csignal<double> &reSig, const csignal<double> &imSig)
{
    if(reSig.size() == imSig.size())
    {
        csignal<double> temp(reSig.size());
        temp.setServiceParameters(reSig.azimuth(), reSig.frameNumber());

        std::transform(reSig.begin(), reSig.end(), imSig.begin(), temp.begin(),
                       [](double reVal, double imVal){return std::arg(std::complex<double>(reVal, imVal));});
        return temp;
    }

    throw csignal<double>::CsignalLengthMismatch();
}

csignal<double> ComplexIm(const csignal<double>& absSig, const csignal<double>& argSig)
{
    if(absSig.size() == argSig.size())
    {
        csignal<double> temp(absSig.size());
        temp.setServiceParameters(absSig.azimuth(), absSig.frameNumber());
        std::transform(absSig.begin(), absSig.end(), argSig.begin(), temp.begin(),
                       [](double absVal, double argVal) { return std::polar(absVal, argVal).imag(); });
        return temp;
    }

    throw csignal<double>::CsignalLengthMismatch();
}

csignal<double> ComplexRe(const csignal<double> &absSig, const csignal<double> &argSig)
{
    if(absSig.size() == argSig.size())
    {
        csignal<double> temp(absSig.size());
        temp.setServiceParameters(absSig.azimuth(), absSig.frameNumber());
        std::transform(absSig.begin(), absSig.end(), argSig.begin(), temp.begin(),
                       [](double absVal, double argVal){ return std::polar(absVal, argVal).real(); });
        return temp;
    }

    throw csignal<double>::CsignalLengthMismatch();
}

csignal<double> AbsComplex(const csignal<std::complex<double>>& Sig)
{
    csignal<double> temp(Sig.size());
    temp.setServiceParameters(Sig.azimuth(), Sig.frameNumber());
    std::transform(Sig.begin(), Sig.end(), temp.begin(),
                   [](const std::complex<double>& s){ return std::abs(s); });
    return temp;
}

csignal<double> ArgComplex(const csignal<std::complex<double>> &Sig)
{
    csignal<double> temp(Sig.size());
    temp.setServiceParameters(Sig.azimuth(), Sig.frameNumber());
    std::transform(Sig.begin(), Sig.end(), temp.begin(),
                   [](const std::complex<double>& x){ return std::arg(x); });
    return temp;
}

void Complex(const csignal<double>& absSig, const csignal<double>& argSig, csignal<std::complex<double> >& result)
{
    result.setServiceParameters(absSig.azimuth(), absSig.frameNumber());
    std::transform(absSig.begin(),absSig.end(),argSig.begin(), result.begin(),
                   [](double absVal, double argVal) { return std::polar(absVal, argVal); });
}

void BiSpectrum(csignal<std::complex<double>>& sig)
{
    std::transform(sig.begin(), sig.end(), sig.begin(),
                   [](std::complex<double>& val){ return val * val * std::conj(val); });
}
