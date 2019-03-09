#include "tone.hpp"

Tone::Tone()
: mRed{}
, mGreen{}
, mBlue{}
, mGrey{}
{
}

Tone::Tone(double red, double green, double blue, double grey)
: mRed{red}
, mGreen{green}
, mBlue{blue}
, mGrey{grey}
{
}

double Tone::red() const
{
    return mRed;
}

double Tone::green() const
{
    return mGreen;
}

double Tone::blue() const
{
    return mBlue;
}

double Tone::grey() const
{
    return mGrey;
}

bool Tone::operator==(const Tone& other) const
{
    return mRed == other.mRed
        && mGreen == other.mGreen
        && mBlue == other.mBlue
        && mGrey == other.mGrey;
}

bool Tone::operator!=(const Tone& other) const
{
    return !operator==(other);
}

bool Tone::operator<(const Tone& other) const
{
    return mRed + mGreen + mBlue + mGrey < other.mRed + other.mGreen + other.mBlue + other.mGrey;
}