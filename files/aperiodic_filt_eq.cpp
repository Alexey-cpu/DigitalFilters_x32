//-----------------------------------------------------------------------------------------------------------------------------------------------
// A.M. Tykvinsky, 13.01.2021
//-----------------------------------------------------------------------------------------------------------------------------------------------
// ‘»Ћ№“– јѕ≈–»ќƒ» » » ¬“ќ–ќ… √ј–ћќЌ» » —ќ ¬—“–ќ≈ЌЌџћ Ё ¬јЋј…«≈–ќћ ƒЋя  ќћѕ≈Ќ—ј÷»» «ј¬јЋј ј„’
//-----------------------------------------------------------------------------------------------------------------------------------------------

#include "aperiodic_filt_eq.hpp"

//-----------------------------------------------------------------------------------------------------------------------------------------------
// конструктор по умолчанию:
aperiodic_filt_eq::aperiodic_filt_eq()
{
	// инициализаци¤ переменных по умолчанию:
	m_Fs       = 4000;
	m_Fn       = 50;
	m_dF       = 5;
	m_d_Amp    = 0;

	m_order    = m_Fs / m_Fn / 2;
	m_ElemNum1 =     m_order + 1;
	m_ElemNum2 = 2 * m_order + 1;
	m_Ts       = 1 / m_Fs;
	
	// инициализаци¤ зеркальных буфферов фильтра:
	m_MBUFF1.BuffInit(m_ElemNum2);
	
	// инициализаци¤ выходных переменных секций фильтра:
	m_out      = 0;
	m_Km       = 0;
	m_pH       = 0;
	m_in_F     = 50;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
// деструктор:
aperiodic_filt_eq::~aperiodic_filt_eq() {}
//-----------------------------------------------------------------------------------------------------------------------------------------------
// функция инициализации фильтра:
int aperiodic_filt_eq::filtInit( double Fs, double Fn, double dF, double d_Amp )
{
	// Fs    - частота дискретизации ј÷ѕ
	// Fn    - номинальна¤ частота сети
	// dF    - допустимое отколонение частоты от номинальной ( задает рабочий частотный диапазон )
	// d_Amp - завал ј„’ каскада фильтров в %

	// инициализаци¤ системных переменных фильтра:
	m_Fs       = Fs;
	m_Fn       = Fn;
	m_dF       = dF;
	m_d_Amp	   = d_Amp;

	m_order    = m_Fs / m_Fn / 2;
	m_ElemNum1 =     m_order + 1;
	m_ElemNum2 = 2 * m_order + 1;
	m_Ts       = 1 / m_Fs;
	m_K1       = ( 1 + 0.19/0.5 );
	m_K2       =  0.19 / 0.5;
	
	// инициализация зеркальных буфферов фильтра:
	m_MBUFF1.BuffInit(m_ElemNum2);
	
	// инициализация выходных переменных секций фильтра:
	m_out      = 0;
	m_Km       = 0;
	m_pH       = 0;
	m_in_F     = Fn;
	
	return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
// функция выделения памяти:
int aperiodic_filt_eq::allocate()
{
	// выделение памяти под зеркальные буфферы фильтра:
	m_MBUFF1.allocate(true);

	// расчет весового коэффициента передаточной функции вспомогательного фильтра:

	// составл¤ем квадратное уравнение дл¤ квадрата ј„’ фильтра - эквалайзера:

	// вспомогательные коэффициенты:
	double A =  0.5 - 0.5 * cos(-6.283185307179586476925286766559 * (m_Fn + m_dF) * (double)m_order * m_Ts );
	double B =  1.0 - cos(-6.283185307179586476925286766559 * (m_Fn + m_dF) * (double)m_order * 2.0 * m_Ts);
	double C = -0.5 * sin(-6.283185307179586476925286766559 * (m_Fn + m_dF) * (double)m_order * m_Ts);
	double D =       -sin(-6.283185307179586476925286766559 * (m_Fn + m_dF) * (double)m_order * 2.0 * m_Ts);

	// коэффициенты уравнени¤:
	double a  = 1;
	double b  = 2 * ( A * B + C * D ) / (B * B + D * D);
	double c  = (A * A + C * C - (1 + m_d_Amp / 100) * (1 + m_d_Amp / 100) ) / (B * B + D * D);

	// решаем квадратное уравнение:
	double Discr  = b * b - 4 * a * c;
	double K1     = 0;
	double K2     = 0;

	if (Discr > 0 ) // на вс¤кий случай провер¤ем, что дискриминант не равен нулю 
	{
		// считаем корни:
		K1 = (-b - sqrt(Discr) ) * 0.5;
		K2 = (-b + sqrt(Discr) ) * 0.5;

		// берем больший корень дл¤ расчета коэффициентов фильтра (вообще, без разницы, какой корень брать дл¤ расчета коэффициентов фильтра):
		m_K1 = (1 + fmax(K1 , K2) / 0.5);
		m_K2 = fmax(K1, K2) / 0.5;
	}
	else // если дискриминант отрицательный, то делаем фильтр без компенсации завала ј„’ 
	{
		m_K1 = 1;
		m_K2 = 0;
	}

	return 0;	
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
// функция освобождения памяти:
int aperiodic_filt_eq::deallocate()
{
	// освобождение памяти под зеркальных буфферов фильтра:
	m_MBUFF1.deallocate();
	return 0;	
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
//функция расчета ј„’ и ‘„’ фильтра:
int aperiodic_filt_eq::FreqCharacteristics( bool mode )
{

	// –асчет ј„’ и ‘„’ фильтра:
	double Re  = 0;
	double Im  = 0;

	if( !mode )
	{
	    Re = m_K1 - cos(-6.283185307179586476925286766559 * (double)m_order * m_in_F * m_Ts) - m_K2 * cos(-6.283185307179586476925286766559 * 2 * (double)m_order * m_in_F * m_Ts);
	    Im = 0    - sin(-6.283185307179586476925286766559 * (double)m_order * m_in_F * m_Ts) - m_K2 * sin(-6.283185307179586476925286766559 * 2 * (double)m_order * m_in_F * m_Ts);
	}
	else
	{
	    Re = m_K1 + cos(-6.283185307179586476925286766559 * (double)m_order * m_in_F * m_Ts) - m_K2 * cos(-6.283185307179586476925286766559 * 2 * (double)m_order * m_in_F * m_Ts);
	    Im = 0    + sin(-6.283185307179586476925286766559 * (double)m_order * m_in_F * m_Ts) - m_K2 * sin(-6.283185307179586476925286766559 * 2 * (double)m_order * m_in_F * m_Ts);
	}

	m_pH = atan2(Im, Re);
	m_Km = sqrt(Re * Re + Im * Im);

	return 0;
};
