#ifndef %PRE_DEF%
#define %PRE_DEF%

#include <%BASECLASS%>

class %CLASS% : public %BASECLASS%
{
    Q_OBJECT

public:
    %CLASS%(QWidget *parent = 0);
    ~%CLASS%();
};

#endif // %PRE_DEF%
