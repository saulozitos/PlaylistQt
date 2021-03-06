#pragma once

#include<QString>

class Settings
{
public:
    static Settings& getInstance();

    QString getClientId() const;
    void setClientId(const QString &value);

    QString getClientSecret() const;
    void setClientSecret(const QString &value);

    QString getRedirect() const;
    void setRedirect(const QString &value);

    QString getCode() const;
    void setCode(const QString &value);

    QString getAccessToken() const;
    void setAccessToken(const QString &value);

    QString getCacheLocation() const;
    void setCacheLocation(const QString &value);

private:
    Settings();
    ~Settings() = default;
    Settings(const Settings&) = delete;
    Settings& operator = (const Settings&) = delete;

    QString cacheLocation;
    QString clientId;
    QString clientSecret;
    QString redirect;
    QString code;
    QString accessToken;
};

