// --------------------------------------------------------------------------------------------------------------
// A.M. Tykvinsky, 04.02.2021
// --------------------------------------------------------------------------------------------------------------
// ТРИГГЕРЫ
// --------------------------------------------------------------------------------------------------------------

#include "triggers.hpp"

// конструктор:
// --------------------------------------------------------------------------------------------------------------
triggers::triggers()
{
	m_Q    = false;
	m_CC   = false;
	m_QQ   = false;
}
// --------------------------------------------------------------------------------------------------------------
//Деструктор:
triggers::~triggers() {}

// --------------------------------------------------------------------------------------------------------------
//Детекция фронта:
int triggers::rr_trig( bool CLK)
{

	if ( (CLK == true) && (m_CC == false) )
	{
		m_Q    = true;
	}
	else
	{
		m_Q = false;
	}

	m_CC = CLK;


	return 0;
}
// --------------------------------------------------------------------------------------------------------------
//Детекция спада:
int triggers::ff_trig( bool CLK)
{

	if ( (CLK == false) && (m_CC == true) )
	{
		m_Q = true;
	}
	else
	{
		m_Q = false;
	}

	m_CC = CLK;

	return 0;
}
// --------------------------------------------------------------------------------------------------------------
// функция RS - триггера:
int triggers::rs_trig( bool S , bool R)
{

	if ( ( (S == true) && (R == false) ) || ( (m_QQ == true) && (R == false) ) )
	{
		m_QQ = true;
		m_Q  = m_QQ;
	}
	else
	{
		m_QQ = false;
		m_Q  = m_QQ;
	}

	return 0;
}
// --------------------------------------------------------------------------------------------------------------
// функция SR - триггера:
int triggers::sr_trig( bool S , bool R)
{

	if ( (S == true) || ( (m_QQ == true) && (R == false) ) )
	{
		m_QQ = true;
		m_Q  = m_QQ;
	}
	else
	{
		m_QQ = false;
		m_Q  = m_QQ;
	}

	return 0;
}
// --------------------------------------------------------------------------------------------------------------
// функция HitCrossing:
int triggers::hit_crossing(double *input, double offset, bool falling)
{
	if (falling == false) 
	{
		rr_trig( (*input > offset) );
	}
	else
	{
		ff_trig( (*input > offset) );
	}

	return 0;
}
// --------------------------------------------------------------------------------------------------------------
// узнать состояние триггера:
bool triggers::get_State() 
{
	return m_Q;
}