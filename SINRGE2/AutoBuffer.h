#ifndef __AUTO_BUFFER_H__
#define __AUTO_BUFFER_H__

//#include <assert.h>

class CAutoBuffer
{
	const static unsigned long uDefaultBufferSize = 4096;

public:
	CAutoBuffer()
		: m_pBuffer(0), m_uLength(0)
	{
		ensure_size(uDefaultBufferSize);
	}

	~CAutoBuffer()
	{
		if (m_pBuffer)
		{
			delete [] m_pBuffer;
			m_pBuffer = 0;
		}
		m_uLength = 0;
	}

public:
	unsigned char*	get_buffer_ptr() const	{ return m_pBuffer; }
	unsigned long	get_buffer_size() const	{ return m_uLength; }

	void			ensure_size(unsigned long uSize)
	{
		if (uSize > m_uLength)
		{
			if (m_pBuffer)
			{
				delete [] m_pBuffer;
				m_pBuffer = 0;
			}
			m_pBuffer = new unsigned char[uSize];
			//assert(m_pBuffer);
			m_uLength = uSize;
		}
	}

private:
	unsigned char*	m_pBuffer;
	unsigned long	m_uLength;
};
#endif	//	__AUTO_BUFFER_H__
