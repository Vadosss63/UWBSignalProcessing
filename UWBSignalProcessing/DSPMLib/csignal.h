#ifndef CSIGNAL_H
#define CSIGNAL_H

#include <cmath>
#include <stdexcept>
#include <iterator>
#include <algorithm>
#include <functional>
#include <valarray>
#include <fstream>
#include <complex>
#include <climits>
#include <cstdlib>
#include <queue>
//windows
#define M_PI 3.14159265358979323846

template <class T> class MovingAverage;

template <class T>
class csignal
{
public:
    //////////////////////////////////////////////////////////////////
    /// вспомогательные классы csignal
    //////////////////////////////////////////////////////////////////
    // класс ошибок csignal
    class CsignalErrors { };

    // несоответствие длинн сигналов
    class CsignalLengthMismatch : public CsignalErrors { };

    // выход за границы сигнала
    class CsignalIndexOutOfRange : public CsignalErrors { };

    // извлечение корня из отрицательного элемента массива
    class CsignalSqrtOfNegativeElements : public CsignalErrors { };

    //////////////////////////////////////////////////////////////////
    /// конструкторы/деструкторы
    //////////////////////////////////////////////////////////////////

    //конструктор по умолчанию. создает сигнал нулевой длины
    csignal();

    //конструктор, создающий сигнал длины size с нулевыми отсчетами
    explicit csignal(size_t size);

    //конструктор, создающий сигнал длины size с заданными отчетами
    csignal(size_t size, const T& element);

    //конструкторы копирования - перемещения
    csignal(const csignal& rhs);
    csignal(csignal&& rhs);

    //деструктор
    ~csignal();

    //////////////////////////////////////////////////////////////////
    /// перегрузка операторов присваивания/индексации
    //////////////////////////////////////////////////////////////////

    //операторы присваивания
    csignal& operator = (const csignal& rhs);
    csignal& operator = (csignal&& rhs);

    //операторы индексирования для доступа к элементу массива
    T& operator[] (size_t index);
    const T& operator[] (size_t index) const;

    //////////////////////////////////////////////////////////////////
    /// перегрузка математических операций
    //////////////////////////////////////////////////////////////////

    //математические операторы над csignal
    csignal<T>& operator += (const csignal& rhs);
    csignal<T>& operator += (const T& val);
    csignal<unsigned char>& operator += (const int val);

    csignal<T>& operator -= (const csignal& rhs);
    csignal<T>& operator -= (const T& val);
    csignal<T>& operator *= (const csignal& rhs);
    csignal<T>& operator *= (double mult);

    csignal<unsigned char>& operator *= (const int mult);
    csignal<T>& operator /= (size_t divide);
    csignal<T>& operator /= (const csignal<T>& divide);

    //операторы сравнения
    bool operator < (const T& thresh);
    bool operator < (const T& thresh) const;
    bool operator > (const T& thresh);
    bool operator > (const T& thresh) const;
    bool operator == (const csignal<T>& signalOther);
    bool operator == (const csignal<T>& signalOther) const;

    //операторы для конкатенации сигналов
    csignal<T>& operator &= (const csignal& rhs);
    csignal<T>& operator &= (csignal&& rhs);

    //////////////////////////////////////////////////////////////////
    /// потоковый ввод/вывод
    //////////////////////////////////////////////////////////////////

    // запись в файл
    template <typename U, typename Tstreem> friend void operator << (Tstreem& out, const csignal<U>& signal);
    // для чтения из файла
    template <typename U, typename Tstreem> friend csignal<U>& operator >> (Tstreem& in, csignal<U>& signal);

    //////////////////////////////////////////////////////////////////
    /// приведение типа
    //////////////////////////////////////////////////////////////////

    template <class U> explicit operator csignal<U>();
    explicit operator csignal<unsigned char>();
//    explicit operator csignal<double>();


    //////////////////////////////////////////////////////////////////
    /// вспомогательные функции сигнала
    //////////////////////////////////////////////////////////////////

    //возвращает количество отсчетов сигнала
    inline size_t size() const;

    //проверка сигнала на наличие элементов в нем
    inline bool empty() const;

    //изменение размера сигнала. При этом старые отсчеты уничтожаются
    void reset(size_t new_size);

    //удаление всех отсчетов сигнала
    inline void clear();

    //получение значения азимута
    inline uint32_t azimuth() const;
    //получение значения дальности 1-го  элемента
    inline uint8_t range() const;
    //получение номера кадра
    inline uint16_t frameNumber() const;

    // получение обьема выборки
    inline uint32_t simpleSizeAzimuth() const;

    //установка азимута
    void setAzimuth(uint32_t az);
    //установка дальности 1-го элемента
    void setRange(uint8_t ran);
    //установка номера кадра
    void setFrameNumber(uint16_t frame);

    //установка значений азимута, номера кадра, номера платы
    void setServiceParameters(uint32_t az, uint16_t frame);

    void apply(T func(T));

    // математические функции
    void abs();
    void exp();
    void sqrt();
    void log();
    void log10();
    void sin();
    void cos();
    void tan();
    void sinh();
    void cosh();
    void tanh();
    void asin();
    void acos();
    void atan();

    void pow(int val);

    void shift(int num);
    void cshift(int num);

    void swap(csignal& signal);

    T max();
    T min();
    T sum();


    csignal<T> SimpleMovingAverage(size_t sizeWindow);

    //Установка и получение размера выборки азимута
    static void setAzimuthVolume(uint32_t simpleSizeAzimuth);
    static uint32_t azimuthVolume();

    //////////////////////////////////////////////////////////////////
    /// итераторы для csignal
    //////////////////////////////////////////////////////////////////

    //методы класса для получения итераторов
    auto begin();
    auto begin() const;
    auto end();
    auto end() const;

private:
    /////////////////////////////////////////////////////////////////////
    /// переменные - члены класса
    /////////////////////////////////////////////////////////////////////

    std::valarray<T> m_signal;            //контейнер с отсчетами сигнала

    uint32_t m_azimuth = 0;	    //значение азимута

    uint8_t m_range = 0;          //Дальность 1-го элемента массива

    uint16_t m_frameNumber = 0;  //номер кадра для синхронизации

    static uint32_t m_simpleSizeAzimuth; //объем выборки азимута
};

template <class T>
void csignal<T>::apply(T func(T))
{
    m_signal = m_signal.apply(func);
}

template <class T>
uint32_t csignal<T>::m_simpleSizeAzimuth = 64;

/////////////////////////////////////////////////////////////////////
/// описание методов класса
/////////////////////////////////////////////////////////////////////

template <class T>
csignal<T>::csignal(){}

template <class T>
csignal<T>::csignal(size_t size)
    : m_signal(size){}


template <class T>
csignal<T>::csignal(size_t size, const T& element)
    : m_signal(element , size){}


template <class T>
csignal<T>::csignal(const csignal& rhs)
    : m_signal(rhs.m_signal),
      m_azimuth(rhs.m_azimuth),
      m_range(rhs.m_range),
      m_frameNumber(rhs.m_frameNumber){}

template <class T>
csignal<T>::csignal(csignal&& rhs)
    : m_signal(std::move(rhs.m_signal)),
      m_azimuth(rhs.m_azimuth),
      m_range(rhs.m_range),
      m_frameNumber(rhs.m_frameNumber){}

template <class T>
csignal<T>::~csignal(){}

template <class T>
csignal<T>& csignal<T>::operator=(const csignal& rhs)
{
    if (this != &rhs)
    {
        m_signal = rhs.m_signal;
        m_azimuth = rhs.m_azimuth;
        m_range = rhs.m_range;
        m_frameNumber = rhs.m_frameNumber;
    }
    return *this;
}

template <class T>
csignal<T>& csignal<T>::operator=(csignal&& rhs)
{
    if (this != &rhs)
    {
        m_signal = std::move(rhs.m_signal);
        m_azimuth = rhs.m_azimuth;
        m_range = rhs.m_range;
        m_frameNumber = rhs.m_frameNumber;
    }
    return *this;
}

template <class T>
T& csignal<T>::operator[](size_t index)
{
    return m_signal[index];
}

template <class T>
const T& csignal<T>::operator[](size_t index) const
{
    return m_signal[index];
}

template <class T>
csignal<T>& csignal<T>::operator+=(const csignal& rhs)
{
    if(m_signal.size() == 0)
    {
        m_signal = rhs.m_signal;
        setServiceParameters(rhs.m_azimuth,  rhs.m_frameNumber);
    }
    else if (m_signal.size() == rhs.size())
    {
        m_signal += rhs.m_signal;
    }
    else
        throw CsignalLengthMismatch();

    return *this;
}

template <class T>
csignal<T>& csignal<T>::operator+=(const T& val)
{  
    m_signal += val;
    return *this;
}

template <class T>
csignal<T>& csignal<T>::operator-=(const csignal& rhs)
{
    if(m_signal.size() == 0)
    {
        m_signal = rhs.m_signal;
        m_signal *= (-1);
        setServiceParameters(rhs.m_azimuth,  rhs.m_frameNumber);
    }
    else if (m_signal.size() == rhs.size())
    {
        m_signal -= rhs.m_signal;

    }
    else
        throw CsignalLengthMismatch();

    return *this;
}

template <class T>
csignal<T>& csignal<T>::operator-=(const T& val)
{
    m_signal -= val;
    return *this;
}

template <class T>
csignal<T>& csignal<T>::operator*=(const csignal& rhs)
{
    if (m_signal.size() == rhs.size())
    {
        m_signal *= rhs.m_signal;
        return *this;
    }
    else
        throw CsignalLengthMismatch();
}

template <class T>
csignal<T>& csignal<T>::operator*=(double mult)
{
    m_signal *= mult;
    return *this;
}

template <class T>
csignal<T>& csignal<T>::operator/=(size_t divide)
{
    m_signal /= divide;
    return *this;
}

template <class T>
csignal<T>& csignal<T>::operator/=(const csignal<T>& divide)
{
    m_signal /= divide.m_signal;
    return *this;
}

template <class T>
csignal<T>& csignal<T>::operator&=(const csignal& rhs)
{
    if(m_signal.size() == 0)
    {
        m_signal = rhs.m_signal;
        setServiceParameters(rhs.azimuth(), rhs.frameNumber());
        return *this;
    }
    std::valarray<T> tmp(m_signal);
    m_signal.resize(tmp.size() + rhs.size());
    std::copy(std::begin(tmp), std::end(tmp), this->begin());
    std::copy(rhs.begin(), rhs.end(), this->begin() + tmp.size());
    return *this;
}

template <class T>
csignal<T>& csignal<T>::operator&=(csignal&& rhs)
{    
    if(m_signal.size() == 0)
    {
        m_signal = std::move(rhs.m_signal);
        setServiceParameters(rhs.azimuth(), rhs.frameNumber(), rhs.board_number());
    }
    std::valarray<T> tmp(m_signal);
    m_signal.resize(tmp.size() + rhs.size());
    std::move(std::begin(tmp), std::end(tmp), this->begin());
    std::move(rhs.begin(), rhs.end(), this->begin() + tmp.size());
    return *this;
}

template <class T>
bool csignal<T>::operator < (const T& thresh)
{
    return std::any_of(std::begin(m_signal), std::end(m_signal),
                       [thresh](const T& element){return element < thresh;});
}

template <class T>
bool csignal<T>::operator < (const T& thresh) const
{
    return std::any_of(std::begin(m_signal), std::end(m_signal),
                       [thresh](const T& element){return element < thresh;});
}

template <class T>
bool csignal<T>::operator > (const T& thresh)
{
    return std::any_of(std::begin(m_signal), std::end(m_signal),
                       [thresh](const T& element){return element > thresh;});
}

template <class T>
bool csignal<T>::operator >(const T& thresh) const
{
    return std::any_of(std::begin(m_signal), std::end(m_signal),
                       [thresh](const T& element){return element > thresh;});
}

template<class T>
bool csignal<T>::operator == (const csignal<T>& signalOther)
{
    if(m_signal.size() != signalOther.m_signal.size()) return false;
    if(m_azimuth != signalOther.m_azimuth) return false;
    if(m_frameNumber != signalOther.m_frameNumber) return false;
    return std::equal(begin(), end(), signalOther.begin());
}

template<class T>
bool csignal<T>::operator == (const csignal<T>& signalOther) const
{
    if(m_signal.size() != signalOther.m_signal.size()) return false;
    if(m_azimuth != signalOther.m_azimuth) return false;
    if(m_frameNumber != signalOther.m_frameNumber) return false;
    return std::equal(begin(), end(), signalOther.begin());
}

template <class T>
size_t csignal<T>::size() const
{
    return m_signal.size();
}

template <class T>
bool csignal<T>::empty() const
{
    return !m_signal.size();
}

template <class T>
void csignal<T>::reset(size_t new_size)
{
    m_signal.resize(new_size);
}

template <class T>
void csignal<T>::clear()
{
    m_signal.resize(0);
}

template <class T>
uint32_t csignal<T>::azimuth() const
{
    return m_azimuth;
}

template <class T>
uint8_t csignal<T>::range() const
{
    return m_range;
}

template <class T>
uint16_t csignal<T>::frameNumber() const
{
    return m_frameNumber;
}

template <class T>
uint32_t csignal<T>::simpleSizeAzimuth() const
{
    return m_simpleSizeAzimuth;
}

template <class T>
void csignal<T>::setAzimuth(uint32_t az)
{
    m_azimuth = az % simpleSizeAzimuth();
}

template <class T>
void csignal<T>::setRange(uint8_t ran)
{
    m_range = ran;
}

template <class T>
void csignal<T>::setFrameNumber(uint16_t frame)
{
    m_frameNumber = frame;
}

template <class T>
void csignal<T>::setServiceParameters(uint32_t az, uint16_t frame)
{
    setAzimuth(az);
    setFrameNumber(frame);
}

template<class T>
void csignal<T>::abs()
{
    m_signal = std::abs(m_signal);
}

template<class T>
void csignal<T>::exp()
{
    m_signal = std::exp(m_signal);
}

template<class T>
void csignal<T>::sqrt()
{
    m_signal = std::sqrt(m_signal);
}

template<class T>
void csignal<T>::log()
{
    m_signal = std::log(m_signal);
}

template<class T>
void csignal<T>::log10()
{
    m_signal = std::log10(m_signal);
}

template<class T>
void csignal<T>::sin()
{
    m_signal = std::sin(m_signal);
}

template<class T>
void csignal<T>::cos()
{
    m_signal = std::cos(m_signal);
}

template<class T>
void csignal<T>::tan()
{
    m_signal = std::tan(m_signal);
}

template<class T>
void csignal<T>::sinh()
{
    m_signal = std::sinh(m_signal);
}

template<class T>
void csignal<T>::cosh()
{
    m_signal = std::cosh(m_signal);
}

template<class T>
void csignal<T>::tanh()
{
    m_signal = std::tanh(m_signal);
}

template<class T>
void csignal<T>::asin()
{
    m_signal = std::asin(m_signal);
}

template<class T>
void csignal<T>::acos()
{
    m_signal = std::acos(m_signal);
}

template<class T>
void csignal<T>::atan()
{
    m_signal = std::atan(m_signal);
}

template<class T>
void csignal<T>::pow(int val)
{
    m_signal = std::pow(m_signal, static_cast<T>(val));
}

template<class T>
void csignal<T>::shift(int val)
{
    m_signal = m_signal.shift(val);
}

template<class T>
void csignal<T>::cshift(int val)
{
    m_signal = m_signal.cshift(val);
}

template<class T>
csignal<T> csignal<T>::SimpleMovingAverage(size_t sizeWindow)
{
    csignal<T> sig(m_signal.size());
    sig.setServiceParameters(m_azimuth, m_frameNumber);
    std::for_each(begin(), end(), MovingAverage<csignal<T>>(sig, sizeWindow));
    return sig;
}

template<class T>
T csignal<T>::max()
{
    return m_signal.max();
}

template<class T>
T csignal<T>::min()
{
    return m_signal.min();
}

template<class T>
T csignal<T>::sum()
{
    return m_signal.sum();
}

template<class T>
void csignal<T>::swap(csignal<T>& signal)
{
    std::swap(m_azimuth, signal.m_azimuth);
    std::swap(m_range, signal.m_range);
    std::swap(m_frameNumber, signal.m_frameNumber);
    m_signal.swap(signal.m_signal);
}

template <class T>
void csignal<T>::setAzimuthVolume(uint32_t simpleSizeAzimuth)
{
    m_simpleSizeAzimuth = simpleSizeAzimuth;
}

template <class T>
uint32_t csignal<T>::azimuthVolume()
{
    return m_simpleSizeAzimuth;
}

template <class T>
auto csignal<T>::begin()
{
    return std::begin(this->m_signal);
}

template <class T>
auto csignal<T>::begin() const
{
    return std::begin(this->m_signal);
}

template <class T>
auto csignal<T>::end()
{
    return std::end(this->m_signal);
}

template <class T>
auto csignal<T>::end() const
{
    return std::end(this->m_signal);
}

template <class T>
template <class U>
csignal<T>::operator csignal<U>()
{
    csignal<U> temp(this->size());
    std::transform(this->begin(), this->end(), temp.begin(),
                   [](T typeT){ return static_cast<U>(typeT); });
    temp.setServiceParameters(this->m_azimuth, this->m_frameNumber);
    return temp;
}

template <>
csignal<double>::operator csignal<unsigned char>();

//template <>
//csignal<unsigned char>::operator csignal<double>();

template <>
csignal<unsigned char>& csignal<unsigned char>::operator += (const int val);

template <>
csignal<unsigned char>& csignal<unsigned char>::operator *= (const int mult);

//глобальные функции
template <class T>
csignal<T> operator+(const csignal<T>& lhs, const T& val)
{
    csignal<T> temp(lhs);
    temp += val;
    return temp;
}

template <class T>
csignal<T> operator+(const T& val , const csignal<T>& rhs )
{
    csignal<T> temp(rhs);
    temp += val;
    return temp;
}

template <class T>
csignal<T> operator+(const csignal<T>& first, const csignal<T>& second)
{
    if(first.size() == second.size())
    {
        csignal<T> temp(first);
        temp += second;
        return temp;
    }
    else
        throw csignal<double>::CsignalLengthMismatch();
}

template <class T>
csignal<T> operator-(const csignal<T>& first, const csignal<T>& second)
{
    if(first.size() == second.size())
    {
        csignal<T> temp(first);
        temp -= second;
        return temp;
    }
    else
        throw csignal<double>::CsignalLengthMismatch();
}

template <class T>
csignal<T> operator-(const csignal<T>& lhs, const T& val)
{
    csignal<T> temp(lhs);
    temp -= val;
    return temp;
}

template <class T>
csignal<T> operator-(const T& val , const csignal<T>& rhs)
{
    csignal<T> temp(rhs);
    temp -= val;
    return temp;
}

template <class T>
csignal<T> operator*(const csignal<T>& first, const csignal<T>& second)
{
    if(first.size() == second.size())
    {
        csignal<T> temp(first);
        temp *= second;
        return temp;
    }
    else
        throw csignal<double>::CsignalLengthMismatch();
}

template <class T>
csignal<T> operator/(const csignal<T>& lhs, const T& rhs)
{
    csignal<T> temp(lhs);
    temp /= rhs;
    return temp;
}

template <class T>
csignal<T> operator / (const csignal<T>& lhs, const csignal<T>& rhs)
{
    if(lhs.size() == rhs.size())
    {
        csignal<T> temp(lhs / rhs);
        temp.setServiceParameters(lhs.azimuth(), lhs.frameNumber(), lhs.board_number());
        return temp;
    }
    else
        throw csignal<double>::CsignalLengthMismatch();
}

template <class T>
csignal<T> operator*(const csignal<T>& lhs, const T& rhs)
{
    csignal<T> temp(lhs);
    temp *= rhs;
    return temp;
}

template <class T>
csignal<T> operator*(const T& lhs, const csignal<T>& rhs)
{

    csignal<T> temp(rhs);
    temp *= lhs;
    return temp;
}

template <class T>
csignal<T> Square(const csignal<T>& sig)
{
    return sig * sig;
}

template <class T>
csignal<T> Corelation(const csignal<T>& first, const csignal<T>& second)
{
    csignal<T> temp(first.size() + second.size() - 1);
    temp.setServiceParameters(first.azimuth(), first.frameNumber());
    for (size_t i = 0; i < first.size(); i++)
        for (size_t j = 0; j < second.size(); j++)
            temp[i+j] += first[i] * second[second.size() - 1 - j];

    return temp;
}

template <class T>
csignal<T> CorelationForFilter(const csignal<T>& sig, const csignal<T>& filter)
{
    csignal<T> temp(sig.size() + filter.size() - 1);
    temp.setServiceParameters(sig.azimuth(), sig.frameNumber());

    for (size_t i = 0; i < sig.size(); i++)
        for (size_t j = 0; j < filter.size(); j++)
            temp[i+j] += sig[i] * filter[j];

    csignal<T> res(sig.size());
    std::move(temp.begin() + (filter.size() - 1), temp.end(), res.begin());
    res.setServiceParameters(sig.azimuth(), sig.frameNumber());
    return res;
}

template <class T>
csignal<T> Convolution(const csignal<T>& first, const csignal<T>& second)
{
    csignal<T> temp(first.size() + second.size() - 1);
    temp.setServiceParameters(first.azimuth(), first.range(), first.frameNumber(), first.board_number());
    for (size_t i = 0; i < first.size(); i++)
        for (size_t j = 0; j < second.size(); j++)
            temp[i + j] += first[i] * second[j];

    return temp;
}

//для чтения из файла
template <typename U, typename Tstreem>
csignal<U>& operator >> (Tstreem& in, csignal<U>& signal)
{
    in.read(reinterpret_cast<char*>(&signal.m_azimuth), sizeof signal.m_azimuth);
    in.read(reinterpret_cast<char*>(&signal.m_range), sizeof signal.m_range);
    in.read(reinterpret_cast<char*>(&signal.m_frameNumber), sizeof signal.m_frameNumber);
    in.read(reinterpret_cast<char*>(&csignal<U>::m_simpleSizeAzimuth), sizeof csignal<U>::m_simpleSizeAzimuth);
    for (auto& sig : signal){in.read(reinterpret_cast<char*>(&sig), sizeof(U));}
    return signal;
}

//шаблон для записи в файл
template <typename U, typename Tstreem>
void operator << (Tstreem& out, const csignal<U>& signal)
{
    out.write(reinterpret_cast<char const*>(&signal.m_azimuth), sizeof signal.m_azimuth);
    out.write(reinterpret_cast<char const*>(&signal.m_range), sizeof signal.m_range);
    out.write(reinterpret_cast<char const*>(&signal.m_frameNumber), sizeof signal.m_frameNumber);
    out.write(reinterpret_cast<char const*>(&csignal<U>::m_simpleSizeAzimuth), sizeof csignal<U>::m_simpleSizeAzimuth);
    for (auto& sig : signal){out.write(reinterpret_cast<char const*>(&sig), sizeof(U));}
}

csignal<double> Sqrt(const csignal<double>& sig);

csignal<double> Atan(const csignal<double>& sig);

csignal<double> Tan(const csignal<double>& sig);

csignal<double> Cos(const csignal<double>& sig);

csignal<double> Sin(const csignal<double>& sig);

csignal<double> ABS(const csignal<double>& sig);

csignal<double> AbsComplex(const csignal<double>& reSig, const csignal<double>& imSig);

csignal<double> ArgComplex(const csignal<double>& reSig, const csignal<double>& imSig);

csignal<double> ComplexIm(const csignal<double>& absSig, const csignal<double>& argSig);

csignal<double> ComplexRe(const csignal<double>& absSig, const csignal<double>& argSig);

csignal<double> AbsComplex(const csignal<std::complex<double>>& Sig);

csignal<double> ArgComplex(const csignal<std::complex<double> > &Sig);

csignal<double> ArgComplex(const csignal<std::complex<double> > &Sig);

void Complex(const csignal<double> &absSig, const csignal<double> &argSig, csignal<std::complex<double> >& result);

void BiSpectrum(csignal<std::complex<double>>& sig);

template <class T>
class MovingAverage
{

public:
    MovingAverage(T& out, size_t period) : m_out(out), m_period(period) {}

    void operator()(double num) {
        m_sum += num;
        m_window.push(num);
        if (m_window.size() > m_period) {
            m_sum -= m_window.front();
            m_window.pop();
        }
        m_out[m_iteretion] = m_sum / m_window.size();
        m_iteretion++;
    }

private:

    T& m_out;
    std::queue<double> m_window; // окно
    size_t m_period; // максимальный размер окна
    double m_sum; // сумма элементов в окне
    size_t m_iteretion = 0;
};

template <class T>
class MovingAverageComplex
{

public:
    MovingAverageComplex(T& out, size_t period) : m_out(out), m_period(period) {}

    void operator()(std::complex<double> num) {

        m_sum += num;
        m_window.push(num);
        if (m_window.size() > m_period) {
            m_sum -= m_window.front();
            m_window.pop();
        }
        m_out[m_iteretion].real() = m_sum.real() / m_window.size();
        m_out[m_iteretion].imag() = m_sum.imag() / m_window.size();

        m_iteretion++;
    }

private:

    T& m_out;
    std::queue<std::complex<double>> m_window; // окно
    size_t m_period; // максимальный размер окна
    std::complex<double> m_sum; // сумма элементов в окне
    size_t m_iteretion = 0;
};


#endif
