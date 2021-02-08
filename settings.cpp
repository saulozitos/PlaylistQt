#include "settings.h"

namespace DevAcount
{
    constexpr auto id = "b782dcea513b41f385613b99a7ad9945";
    constexpr auto secret = "";
    constexpr auto addrRedirect = "http://localhost:8888";
}

Settings::Settings() :
    clientId(DevAcount::id),
    clientSecret(DevAcount::secret),
    redirect(DevAcount::addrRedirect),
    code(QStringLiteral("")),
    accessToken(QStringLiteral("")),
    refreshToken(QStringLiteral(""))
{}

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

QString Settings::getRefreshToken() const
{
    return refreshToken;
}

void Settings::setRefreshToken(const QString &value)
{
    refreshToken = value;
}

void Settings::clearSettings()
{
    clientId.clear();
    clientSecret.clear();
    redirect.clear();
    code.clear();
    accessToken.clear();
    refreshToken.clear();
    accessTokenExpiryTime.zero();
}

std::chrono::seconds Settings::getAccessTokenExpiryTime() const
{
    return accessTokenExpiryTime;
}

void Settings::setAccessTokenExpiryTime(const int value)
{
    accessTokenExpiryTime = std::chrono::seconds(value);
}

