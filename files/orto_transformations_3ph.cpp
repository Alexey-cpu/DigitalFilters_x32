// A.M. Tykvinsky, 27.11.2020
//-----------------------------------------------------------
// ТРЕХФАЗНЫЕ ОРТОГОНАЛЬНЫЕ ПРЕОБРАЗОВАНИЯ:
//-----------------------------------------------------------

#include "orto_transformations_3ph.hpp"

// ---------------------------------------------------
// конструктор по умолчанию
orto_transformations_3ph::orto_transformations_3ph()
{
	// инициализация выходных переменных:
	m_A   = 0;
	m_B   = 0;
	m_D   = 0;
	m_Q   = 0;
	m_Z	  = 0;
	m_cnt = 0;
}
// ---------------------------------------------------
// деструктор
orto_transformations_3ph::~orto_transformations_3ph(){}
// ---------------------------------------------------
// функция преобразования Кларка:
int orto_transformations_3ph::Clark(double *A, double *B , double *C)
{
	// *A - указатель на массив с выборкой сигнала фазы А
	// *В - указатель на массив с выборкой сигнала фазы В
	// *С - указатель на массив с выборкой сигнала фазы С

	// Преобразование Кларка:
	m_A = (*A)*0.6666666666666666666666666666667 - (*B) * 0.3333333333333333333333333333333 - (*C) * 0.3333333333333333333333333333333;
	m_B = (*B) * 0.577350269189625764509148780502 - (*C) * 0.577350269189625764509148780502;
	m_Z = (*A) * 0.3333333333333333333333333333333 + (*B) * 0.3333333333333333333333333333333 + (*C) * 0.3333333333333333333333333333333;

	return 0;
}
// ---------------------------------------------------
// функция преобразования Парка:
int orto_transformations_3ph::Park(double *A, double *B, double *C, double Fn , double Fs, bool A_axis_alignment)
{
	// *A                - указатель на массив с выборкой сигнала фазы А
	// *В                - указатель на массив с выборкой сигнала фазы В
	// *С                - указатель на массив с выборкой сигнала фазы С
	//  Fn               - номинальная частота опорной синусоиды
	//  Fs               - частота дискретизации выборки
	//  A_axis_alignment - совмещение оси d с осью фазы А

	// преобразование Парка:
	if (A_axis_alignment == true)
	{
		m_D = 0.6666666666666666666666666666667*((*A) * cos(PI2 * Fn * (double)m_cnt / Fs) + (*B) * cos(PI2 * Fn * (double)m_cnt / Fs - 2 * PI0 / 3) + (*C) * cos(PI2 * Fn * (double)m_cnt / Fs + 2 * PI0 / 3));
		m_Q = 0.6666666666666666666666666666667 *(-(*A) * sin(PI2 * Fn * (double)m_cnt / Fs) - (*B) * sin(PI2 * Fn * (double)m_cnt / Fs - 2 * PI0 / 3) - (*C) * sin(PI2 * Fn * (double)m_cnt / Fs + 2 * PI0 / 3));
		m_Z = (*A) * 0.3333333333333333333333333333333 + (*B) * 0.3333333333333333333333333333333 + (*C) * 0.3333333333333333333333333333333;
	}
	else 
	{
		m_D = 0.6666666666666666666666666666667*((*A) * sin(PI2 * Fn * (double)m_cnt / Fs) + (*B) * sin(PI2 * Fn * (double)m_cnt / Fs - 2 * PI0 / 3) + (*C) * sin(PI2 * Fn * (double)m_cnt / Fs + 2 * PI0 / 3));
		m_Q = 0.6666666666666666666666666666667*((*A) * cos(PI2 * Fn * (double)m_cnt / Fs) + (*B) * cos(PI2 * Fn * (double)m_cnt / Fs - 2 * PI0 / 3) + (*C) * cos(PI2 * Fn * (double)m_cnt / Fs + 2 * PI0 / 3));
		m_Z = (*A) * 0.3333333333333333333333333333333 + (*B) * 0.3333333333333333333333333333333 + (*C) * 0.3333333333333333333333333333333;
	}

	// обновление значения счетчика:
	// если счетчик периодический не обнулять, то на устройстве произойдет ошибка переполнения "Overflow Error" !!!
	if (m_cnt < Fs / Fn)
	{
		m_cnt++;
	}
	else 
	{
		m_cnt = 0;
	}

	return 0;
}