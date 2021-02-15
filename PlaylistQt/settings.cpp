#include "settings.h"

namespace DevAcount
{
    constexpr auto addrRedirect = "http://localhost:8888";
}

Settings::Settings() :
    cacheLocation(QLatin1String("")),
    clientId(QLatin1String("")),
    clientSecret(QLatin1String("")),
    redirect(DevAcount::addrRedirect),
    code(QLatin1String("")),
    accessToken(QLatin1String(""))
{}

QString Settings::getCacheLocation() const
{
    return cacheLocation;
}

void Settings::setCacheLocation(const QString &value)
{
    cacheLocation = value;
}

Settings &Settings::getInstance()
{
    static Settings instance;
    return instance;
}

QString Settings::getClientId() const
{
    return clientId;
}

void Settings::setClientId(const QString &value)
{
    clientId = value;
}

QString Settings::getClientSecret() const
{
    return clientSecret;
}

void Settings::setClientSecret(const QString &value)
{
    clientSecret = value;
}

QString Settings::getRedirect() const
{
    return redirect;
}

void Settings::setRedirect(const QString &value)
{
    redirect = value;
}

QString Settings::getCode() const
{
    return code;
}

void Settings::setCode(const QString &value)
{
    code = value;
}

QString Settings::getAccessToken() const
{
    return accessToken;
}

void Settings::setAccessToken(const QString &value)
{
    accessToken = value;
}
