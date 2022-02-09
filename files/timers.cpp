// --------------------------------------------------------------------------------------------------------------
// A.M. Tykvinsky , 04.02.2021
// --------------------------------------------------------------------------------------------------------------
// ТАЙМЕРЫ
// --------------------------------------------------------------------------------------------------------------

#include "timers.hpp"

// --------------------------------------------------------------------------------------------------------------
// конструктор (по умолчанию):
timers::timers()
{
	m_Fs             = 4000;
	m_Ts             = 1 / m_Fs;
	m_FramesPerCycle = 16;
	m_Time           = 0;
	m_Q              = false;
	m_CC			 = false;
	m_QQ             = false;
}
// --------------------------------------------------------------------------------------------------------------
// деструктор:
timers::~timers() {}

// --------------------------------------------------------------------------------------------------------------
// функция инициализации:
int timers::Init(double Fs, int FramesPerCycle)
{
	// инициализация системных переменных:
	m_Fs             = Fs;
	m_Ts             = 1 / m_Fs;
	m_FramesPerCycle = FramesPerCycle;
	m_Time           = 0;
	m_Q              = false;
	m_Q              = false;
	m_CC             = false;
	m_QQ             = false;

	return 0;
}

// --------------------------------------------------------------------------------------------------------------
// таймер на фронт:
int timers::ton(bool S , double dT)
{
	if ( (S == true) && (m_Q == false) ) // Если входной сигнал true, то считаем выдержку времени
	{
		for ( int n = 0 ; n < m_FramesPerCycle; n++) // выдержка считается в секундах внутри такта
		{
			m_Time = m_Time + m_Ts;
			
			if (m_Time > dT) 
			{
				break;
			}
		}
	}
	else          // Если входной сигнал false, то обнуляем выдержку времени
	{
		m_Time = 0;
	}

	if (m_Time > dT && S == true) //срабатыване при истечении выдержки и сохранении условий срабатывания
	{
		m_Q = true;
	}
	else if( S == false )
	{
		m_Q = false;
	}
	
	return 0;
}
// --------------------------------------------------------------------------------------------------------------
// таймер на спад:
int timers::tof( bool S , double dT)
{
	if ( S == true ) 
	{
		m_Q    = true;
		m_Time = 0;
	}

	if ( m_Time > dT && S == false )
	{
		m_Q    = false;
		m_Time = 0;
	}

	if( (m_Q == true) && (S == false) )
	{
		for (int n = 0; n < m_FramesPerCycle; n++) // выдержка считается в секундах внутри такта
		{
			m_Time = m_Time + 1 / m_Fs;

			if (m_Time > dT)
			{
				break;
			}
		}	
	}

	return 0;
}
// --------------------------------------------------------------------------------------------------------------
//
int timers::tp( bool S , double dT)
{
	// детекция фронта:
	if ( (S == true) && (m_CC == false) )
	{
		m_QQ = true;
	}
	else
	{
		m_QQ = false;
	}

	m_CC = S;

	// фиксация фронта:
	if (m_QQ == true) 
	{
		m_Q    = true;
		m_Time = 0;
	}

	// отсчет выдержки времени:
	if ( m_Time > dT )
	{
		m_Q    = false;
		m_Time = 0;
	}
	else if( m_Q == true )
	{
		for (int n = 0; n < m_FramesPerCycle; n++) // выдержка считается в секундах внутри такта
		{
			m_Time = m_Time + m_Ts;

			if (m_Time > dT)
			{
				break;
			}
		}
	}

	return 0;
}
// ------------------------------------------------------------------
// Получить значение выхода:
bool timers::getState()
{
	return m_Q;
}
// ------------------------------------------------------------------
// Получить значение выхода:
double timers::getTime()
{
	return m_Time;
}
// ------------------------------------------------------------------