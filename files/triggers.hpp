// --------------------------------------------------------------------------------------------------------------
// A.M. Tykvinsky, 04.02.2021
// --------------------------------------------------------------------------------------------------------------
// ТРИГГЕРЫ
// --------------------------------------------------------------------------------------------------------------

#ifndef _TRIGGERS_HPP
#define _TRIGGERS_HPP

class triggers
{
	
private:

	bool m_CC;   // переменная для запоминания входа детекторов фронта/спада
	bool m_QQ;  // переменная для запоминания входа бистабильных триггеров
	bool m_Q;  // выходная переменная

public:

	// конструктор:
	triggers();

	// деструктор:
	~triggers();

	//
	bool get_State();

	// функции триггеров:
	int rr_trig( bool CLK );         // детектор фронта
	int ff_trig( bool CLK );         // детектор спада
	int rs_trig( bool S , bool R );  // бистабильный RS-триггер
	int sr_trig( bool S , bool R );  // бистабильный SR-триггер

	// функция HitCrossing:
	int hit_crossing(double *input , double offset , bool falling);


};

#endif 


