// dotnet/decompiler/humanizer/vocabularies.cxx -- see vocabularies.hpp.
#include "dotnet/decompiler/humanizer/vocabularies.hpp"

#include <memory>

namespace cb::dotnet::decompiler::humanizer::vocabularies {

namespace {

std::unique_ptr<Vocabulary> build_default() {
    auto default_vocabulary = std::make_unique<Vocabulary>();
    Vocabulary& _default = *default_vocabulary;

    _default.add_plural("$", "s");
    _default.add_plural("s$", "s");
    _default.add_plural("(ax|test)is$", "$1es");
    _default.add_plural("(octop|vir|alumn|fung|cact|foc|hippopotam|radi|stimul|syllab|nucle)us$", "$1i");
    _default.add_plural("(alias|bias|iris|status|campus|apparatus|virus|walrus|trellis)$", "$1es");
    _default.add_plural("(buffal|tomat|volcan|ech|embarg|her|mosquit|potat|torped|vet)o$", "$1oes");
    _default.add_plural("([dti])um$", "$1a");
    _default.add_plural("sis$", "ses");
    _default.add_plural("(?:([^f])fe|([lr])f)$", "$1$2ves");
    _default.add_plural("(hive)$", "$1s");
    _default.add_plural("([^aeiouy]|qu)y$", "$1ies");
    _default.add_plural("(x|ch|ss|sh)$", "$1es");
    _default.add_plural("(matr|vert|ind|d)(ix|ex)$", "$1ices");
    _default.add_plural("(^[m|l])ouse$", "$1ice");
    _default.add_plural("^(ox)$", "$1en");
    _default.add_plural("(quiz)$", "$1zes");
    _default.add_plural("(buz|blit|walt)z$", "$1zes");
    _default.add_plural("(hoo|lea|loa|thie)f$", "$1ves");
    _default.add_plural("(alumn|alg|larv|vertebr)a$", "$1ae");
    _default.add_plural("(criteri|phenomen)on$", "$1a");

    _default.add_singular("s$", "");
    _default.add_singular("(n)ews$", "$1ews");
    _default.add_singular("([dti])a$", "$1um");
    _default.add_singular("(analy|ba|diagno|parenthe|progno|synop|the|ellip|empha|neuro|oa|paraly)ses$", "$1sis");
    _default.add_singular("([^f])ves$", "$1fe");
    _default.add_singular("(hive)s$", "$1");
    _default.add_singular("(tive)s$", "$1");
    _default.add_singular("([lr]|hoo|lea|loa|thie)ves$", "$1f");
    _default.add_singular("(^zomb)?([^aeiouy]|qu)ies$", "$2y");
    _default.add_singular("(s)eries$", "$1eries");
    _default.add_singular("(m)ovies$", "$1ovie");
    _default.add_singular("(x|ch|ss|sh)es$", "$1");
    _default.add_singular("(^[m|l])ice$", "$1ouse");
    _default.add_singular("(?<!^[a-z])(o)es$", "$1");
    _default.add_singular("(shoe)s$", "$1");
    _default.add_singular("(cris|ax|test)es$", "$1is");
    _default.add_singular("(octop|vir|alumn|fung|cact|foc|hippopotam|radi|stimul|syllab|nucle)i$", "$1us");
    _default.add_singular("(alias|bias|iris|status|campus|apparatus|virus|walrus|trellis)es$", "$1");
    _default.add_singular("^(ox)en", "$1");
    _default.add_singular("(matr|d)ices$", "$1ix");
    _default.add_singular("(vert|ind)ices$", "$1ex");
    _default.add_singular("(quiz)zes$", "$1");
    _default.add_singular("(buz|blit|walt)zes$", "$1z");
    _default.add_singular("(alumn|alg|larv|vertebr)ae$", "$1a");
    _default.add_singular("(criteri|phenomen)a$", "$1on");
    _default.add_singular("([b|r|c]ook|room|smooth)ies$", "$1ie");

    _default.add_irregular("person", "people");
    _default.add_irregular("man", "men");
    _default.add_irregular("human", "humans");
    _default.add_irregular("child", "children");
    _default.add_irregular("sex", "sexes");
    _default.add_irregular("glove", "gloves");
    _default.add_irregular("move", "moves");
    _default.add_irregular("goose", "geese");
    _default.add_irregular("wave", "waves");
    _default.add_irregular("foot", "feet");
    _default.add_irregular("tooth", "teeth");
    _default.add_irregular("curriculum", "curricula");
    _default.add_irregular("database", "databases");
    _default.add_irregular("zombie", "zombies");
    _default.add_irregular("personnel", "personnel");
    _default.add_irregular("cache", "caches");
    _default.add_irregular("ex", "exes", false);
    _default.add_irregular("is", "are", false);
    _default.add_irregular("was", "were", false);
    _default.add_irregular("that", "those", false);
    _default.add_irregular("this", "these", false);
    _default.add_irregular("bus", "buses", false);
    _default.add_irregular("die", "dice", false);
    _default.add_irregular("tie", "ties", false);
    _default.add_irregular("lens", "lenses");
    _default.add_irregular("clove", "cloves");
    _default.add_irregular("valve", "valves");
    _default.add_irregular("explosive", "explosives");

    _default.add_uncountable("staff");
    _default.add_uncountable("training");
    _default.add_uncountable("equipment");
    _default.add_uncountable("information");
    _default.add_uncountable("corn");
    _default.add_uncountable("milk");
    _default.add_uncountable("rice");
    _default.add_uncountable("money");
    _default.add_uncountable("species");
    _default.add_uncountable("series");
    _default.add_uncountable("fish");
    _default.add_uncountable("sheep");
    _default.add_uncountable("deer");
    _default.add_uncountable("aircraft");
    _default.add_uncountable("oz");
    _default.add_uncountable("tsp");
    _default.add_uncountable("tbsp");
    _default.add_uncountable("ml");
    _default.add_uncountable("l");
    _default.add_uncountable("water");
    _default.add_uncountable("waters");
    _default.add_uncountable("semen");
    _default.add_uncountable("sperm");
    _default.add_uncountable("bison");
    _default.add_uncountable("grass");
    _default.add_uncountable("hair");
    _default.add_uncountable("mud");
    _default.add_uncountable("elk");
    _default.add_uncountable("luggage");
    _default.add_uncountable("moose");
    _default.add_uncountable("offspring");
    _default.add_uncountable("salmon");
    _default.add_uncountable("shrimp");
    _default.add_uncountable("someone");
    _default.add_uncountable("swine");
    _default.add_uncountable("trout");
    _default.add_uncountable("tuna");
    _default.add_uncountable("corps");
    _default.add_uncountable("scissors");
    _default.add_uncountable("means");
    _default.add_uncountable("mail");
    _default.add_uncountable("pliers");
    _default.add_uncountable("sheers");
    _default.add_uncountable("clothes");
    _default.add_uncountable("apparatus");
    _default.add_uncountable("chassis");
    _default.add_uncountable("debris");

    // Fix 1132
    _default.add_uncountable("metadata");

    return default_vocabulary;
}

}  // namespace

const Vocabulary& get_default() {
    static const std::unique_ptr<Vocabulary> instance = build_default();
    return *instance;
}

}  // namespace cb::dotnet::decompiler::humanizer::vocabularies
