//-----------------------------------------------------------------------------------------------------------------------------------------------
// A.M. Tykvinsky, 13.01.2021
//-----------------------------------------------------------------------------------------------------------------------------------------------
// фИЛЬТР АПЕРИОДИЧЕСКОЙ СЛАГАЮЩЕЙ СО ВСТРОЕННЫМ ЭКВАЛАЙЗЕРОМ
//-----------------------------------------------------------------------------------------------------------------------------------------------

#ifndef _APERIODIC_FILT_EQ_HPP
#define _APERIODIC_FILT_EQ_HPP

#include "mirror_ring_buff_x32.hpp"

class aperiodic_filt_eq
{

private:

	// системные переменные:
	int    m_order;    // пор¤док фильтра
	int    m_ElemNum1; // номер элемента буффера основного дл¤ фильтра
	int    m_ElemNum2; // номер элемента буффера основного дл¤ вспомогательного фильтра
	double m_dF;       // допустимое скольжение ( задает рабочий частотный диапазон )
	double m_d_Amp;    // завал ј„’, который нужно компенсировать
	double m_Fn;       // номинальна¤ частота сети
	double m_Fs;       // частота дискретизации ј÷ѕ
	double m_Ts;       // период  дискретизации ј÷ѕ
	double m_K1;       // коэффициент фильтра
	double m_K2;       // коэффициент фильтра
	
	// зеркальные буфферы:
	mirror_ring_buff m_MBUFF1;

public:

	// входы фильтра:
	double m_in_F;
	
	// выходы фильтра:
	double m_out;
	double m_pH;
	double m_Km;
		
	// функция инициализации фильтра:	
	int filtInit( double Fs , double Fn , double dF , double d_Amp );
	
	// функция выделения памяти:
	int allocate();
	
	// функци¤ освобождени¤ пам¤ти:
	int deallocate();
	
	// конструктор по умолчанию
	aperiodic_filt_eq();

	// деструктор
	~aperiodic_filt_eq();
	
	// функци¤ расчета ј„’ и ‘„’:
	int FreqCharacteristics(bool mode = false);
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------
	// функция фильтрации:
	/*inline int filt(double *input)
	{	
		// прогон сигнала через фильтр:
		m_MBUFF1.fill_buff(input , 1);
		m_out = *input * m_K1 - m_MBUFF1.m_ptr_fill_down[m_ElemNum1] - m_MBUFF1.m_ptr_fill_down[m_ElemNum2] * m_K2 ;
		
		return 0;
	}*/

	inline int filt( double *input , bool odd = true )
	{
		// прогон сигнала через фильтр:
		m_MBUFF1.fill_buff(input );

		if( odd ) // нечетный фильтр
		{
		    m_out = *input * m_K1 - m_MBUFF1.m_ptr_fill_down[m_ElemNum1] - m_MBUFF1.m_ptr_fill_down[m_ElemNum2] * m_K2 ;
		}
		else
		{
		    m_out = *input * m_K1 + m_MBUFF1.m_ptr_fill_down[m_ElemNum1] - m_MBUFF1.m_ptr_fill_down[m_ElemNum2] * m_K2 ;
		}

		return 0;
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------------

};

#endif