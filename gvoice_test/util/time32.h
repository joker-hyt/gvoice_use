#ifndef     __BIBO_FRAME_TIME32_H__
#define     __BIBO_FRAME_TIME32_H__

#include <time.h>

class time32
{
private:
	int m_nTime;
public:
	time32() : m_nTime(0) {};
	time32(int t) : m_nTime(t) {};
	time32(const time32& t) : m_nTime(t.m_nTime) {};
	//time32(const time_t& t) : m_nTime(static_cast<int>(t)) {};

	~time32() {};

	time32& operator= (const time32& t)
	{
		m_nTime = t.m_nTime;
		return *this;
	}

	time32& operator= (int nTime)
	{
		m_nTime = nTime;
		return *this;
	}

	//time32& operator= (const time_t& t)
	//{
	//	m_nTime = static_cast<int>(t);
	//	return *this;
	//}

	bool operator == (const time32& t) const
	{
		return m_nTime == t.m_nTime;
	}

	bool operator == (int nTime) const
	{
		return m_nTime == nTime;
	}

	bool operator != (const time32& t) const
	{
		return m_nTime != t.m_nTime;
	}

	bool operator != (int nTime) const
	{
		return m_nTime != nTime;
	}

	time32 operator + (const time32& t) const
	{
		return time32(m_nTime + t.m_nTime);
	}

	time32 operator - (const time32& t) const
	{
		return time32(m_nTime - t.m_nTime);
	}

	time32& operator += (const time32& t)
	{
		m_nTime += t.m_nTime;
		return( *this );
	}

	time32& operator -= (const time32& t)
	{
		m_nTime -= t.m_nTime;
		return( *this );
	}

	bool operator > (const time32& t) const
	{
		return m_nTime > t.m_nTime;
	}

	bool operator >= (const time32& t) const
	{
		return m_nTime >= t.m_nTime;
	}

	bool operator < (const time32& t) const
	{
		return m_nTime < t.m_nTime;
	}

	bool operator <= (const time32& t) const
	{
		return m_nTime <= t.m_nTime;
	}

	friend time32 operator + (const time32& t32, int tInt) 
	{
		return time32(t32.m_nTime + tInt);
	};

	friend time32 operator + (int tInt, const time32& t32) 
	{
		return time32(t32.m_nTime + tInt);
	};

	friend time32 operator - (const time32& t32, int tInt) 
	{
		return time32(t32.m_nTime - tInt);
	};

	friend time32 operator - (int tInt, const time32& t32) 
	{
		return time32(tInt - t32.m_nTime);
	};


	friend bool operator > (const time32& t32, int tInt) 
	{
		return (t32.m_nTime > tInt);
	};
	
	friend bool operator > (int tInt, const time32& t32) 
	{
		return (tInt > t32.m_nTime);
	};
	friend bool operator >= (const time32& t32, int tInt) 
	{
		return (t32.m_nTime >= tInt);
	};

	friend bool operator >= (int tInt, const time32& t32) 
	{
		return (tInt >= t32.m_nTime);
	};
	
	friend bool operator < (const time32& t32, int tInt) 
	{
		return (t32.m_nTime < tInt);
	};

	friend bool operator < (int tInt, const time32& t32) 
	{
		return (tInt < t32.m_nTime);
	};

	friend bool operator <= (const time32& t32, int tInt) 
	{
		return (t32.m_nTime <= tInt);
	};
	
	friend bool operator <= (int tInt, const time32& t32) 
	{
		return (tInt <= t32.m_nTime);
	};

	friend bool operator == (int tInt, const time32& t32) 
	{
		return (tInt == t32.m_nTime);
	};

public:
	int value() const {return m_nTime;};
	int& value_ref(){return m_nTime;}

	static time32 time(time_t* _Time) 
	{
		return time32(static_cast<int>(::time(_Time)));
	}

	static time32 mktime(tm* _TM)
	{
		return time32(static_cast<int>(::mktime(_TM)));
	}

};


#endif      //__BIBO_FRAME_TIME32_H__
