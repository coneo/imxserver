#ifndef _USER_H__
#define _USER_H__

#include "Type.h"

class User
{
public:
	User( );
	User(ID_t id );
	virtual ~User( );

	ID_t		UserID() const { return m_UserID; }
	void		SetUserID(ID_t id) { m_UserID = id;}

private:
	ID_t			m_UserID;
};
#endif
