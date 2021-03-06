#include "reader/types/armor.hpp"

rpg::Armor::Armor(const Object& object)
{
    if (object.className() != "RPG::Armor")
        throw std::runtime_error(fmt::format("Invalid class name: {}", object.className()));

    mAgilityBonus = *object["@agi_plus"].as<i32>();
    mAutoStateID = *object["@auto_state_id"].as<i32>();
    mDescription = *object["@description"].as<std::string>();
    mDexterityBonus = *object["@dex_plus"].as<i32>();
    mEvasionCorrection = *object["@eva"].as<i32>();

    auto* guardElementSet = object["@guard_element_set"].as<Array>();
    for (const auto& e : *guardElementSet)
        mGuardElementSet.push_back(*e.as<i32>());

    auto* guardStateSet = object["@guard_state_set"].as<Array>();
    for (const auto& e : *guardStateSet)
        mGuardStateSet.push_back(*e.as<i32>());

    mIconName = *object["@icon_name"].as<std::string>();
    mID = *object["@id"].as<i32>();
    mIntelligenceBonus = *object["@int_plus"].as<i32>();
    mKind = static_cast<Kind>(*object["@kind"].as<i32>());
    mMagicDefense = *object["@mdef"].as<i32>();
    mName = *object["@name"].as<std::string>();
    mPhysicalDefense = *object["@pdef"].as<i32>();
    mPrice = *object["@price"].as<i32>();
    mStrengthBonus = *object["@str_plus"].as<i32>();
}

bool rpg::Armor::operator==(const Armor& other) const
{
    return mID == other.mID;
}

bool rpg::Armor::operator!=(const Armor& other) const
{
    return !operator==(other);
}

bool rpg::Armor::operator<(const Armor& other) const
{
    return mID < other.mID;
}

void rpg::to_json(json& j, const Armor& o)
{
    j = json{
        JSON_SET(AgilityBonus),
        JSON_SET(AutoStateID),
        JSON_SET(Description),
        JSON_SET(DexterityBonus),
        JSON_SET(EvasionCorrection),
        JSON_SET(GuardElementSet),
        JSON_SET(GuardStateSet),
        JSON_SET(IconName),
        JSON_SET(ID),
        JSON_SET(IntelligenceBonus),
        JSON_SET(Kind),
        JSON_SET(MagicDefense),
        JSON_SET(Name),
        JSON_SET(PhysicalDefense),
        JSON_SET(Price),
        JSON_SET(StrengthBonus)
    };
}

void rpg::from_json(const json& j, Armor& o)
{
    JSON_GET(AgilityBonus);
    JSON_GET(AutoStateID);
    JSON_GET(Description);
    JSON_GET(DexterityBonus);
    JSON_GET(EvasionCorrection);
    JSON_GET(GuardElementSet);
    JSON_GET(GuardStateSet);
    JSON_GET(IconName);
    JSON_GET(ID);
    JSON_GET(IntelligenceBonus);
    JSON_GET(Kind);
    JSON_GET(MagicDefense);
    JSON_GET(Name);
    JSON_GET(PhysicalDefense);
    JSON_GET(Price);
    JSON_GET(StrengthBonus);
}
