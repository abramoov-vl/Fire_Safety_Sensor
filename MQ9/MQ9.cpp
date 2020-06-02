#include "MQ9.h"

MQ9::MQ9 (PinName const &p) : _pin(p)
{
    _LPG_ppm = 0.0;
    _CO_ppm = 0.0;
    _Methane_ppm = 0.0;
}

int MQ9::read()
{
    float sensorDat = _pin.read();
    float sensorVolt = sensorDat * 3.3f / 4095.0f;
    
    _LPG_ppm = 26.572f*exp(1.2894f*sensorVolt);
    _CO_ppm = 10.938f*exp(1.7742f*sensorVolt);
    _Methane_ppm = 3.027f*exp(1.0698f*sensorVolt);

    return MQ9_STATUS_PASS;
}

float MQ9::getLPG_ppm()
{
    return _LPG_ppm;
}

float MQ9::getCO_ppm()
{
    return _CO_ppm;
}

float MQ9::getMethane_ppm()
{
    return _Methane_ppm;
}