#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>

using namespace std;

class PhysicalItem {
public:
    string name;
    int healthAffectValue{};
    virtual ~PhysicalItem() = default;
};

template <typename T>
concept DerivedFromPhysicalItem = is_base_of_v<PhysicalItem, T>;

template<DerivedFromPhysicalItem T>
class Container {
protected:
    vector<shared_ptr<T>> content;
public:
    [[nodiscard]] vector<shared_ptr<T>>& getContent() { return content; }
    [[nodiscard]] int size() const { return content.size(); }
    void addItem(const shared_ptr<T>& item) {
        content.push_back(item);
    }
    virtual ~Container() = default;
};

class Character {
public:
    string name;
    int hp{};
    virtual bool attack(Character& target, const string& weaponName) {
        cout << name << " attacks " << target.name << " with " << weaponName << "!" << endl;
    }
    virtual ~Character() = default;
    virtual void show(const string& itemType) {
        cout << "Showing items for character " << name << " not implemented." << endl;
    }
};


class Weapon : public PhysicalItem {
};

class Potion : public PhysicalItem {
};

class Spell : public PhysicalItem {
public:
    vector<shared_ptr<Character>> owners;
    void addOwner(const shared_ptr<Character>& owner) {
        owners.push_back(owner);
    }
};

class Arsenal : public Container<Weapon> {
public:
    void showItems(const string& itemType){
        if (itemType == "weapons" || itemType == "weapon") {
            for (const auto& item : content) {
                cout << item->name <<":"<<item->healthAffectValue;
            }
            cout << endl;
        } else {
            cout << "Error: No items of type " << itemType << " in Arsenal." << endl;
        }
    }
};

class MedicalBag : public Container<Potion> {
public:
    void showItems(const string& itemType){
        if (itemType == "potions" || itemType == "potion") {
            for (const auto& item : content) {
                cout << item->name <<":"<<item->healthAffectValue << " ";
            }
            cout << endl;
        } else {
            cout << "Error: No items of type " << itemType << " in MedicalBag." << endl;
        }
    }
};

class SpellBook : public Container<Spell> {
public:
    void showItems(const string& itemType){
        if (itemType == "spells" || itemType == "spell") {
            for (const auto& item : content) {
                cout << item->name <<":"<<item->healthAffectValue;
            }
            cout << endl;
        } else {
            cout << "Error: No items of type " << itemType << " in SpellBook." << endl;
        }
    }
};

class Narrator: public Character {
public:
    static Narrator& getInstance() {
        static Narrator instance;
        return instance;
    }
    Narrator(const Narrator&) = delete;
    Narrator& operator=(const Narrator&) = delete;
private:
    Narrator() = default;
};

class Fighter : public Character {
public:
    Arsenal arsenal;
    MedicalBag bag;
    void addItem(const shared_ptr<PhysicalItem>& Item) {
        if (auto weapon = dynamic_pointer_cast<Weapon>(Item)) {
            if (arsenal.size() < 3) {
                arsenal.addItem(weapon);
                cout << name << " just obtained a new weapon called " << weapon->name << "." << endl;
            } else {
                cout << "Error caught" << endl;
            }
        }
        else if (auto potion = dynamic_pointer_cast<Potion>(Item)) {
            if (bag.size() < 5) {
                bag.addItem(potion);
                cout << name << " just obtained a new potion called " << potion->name << "." << endl;
            } else {
                cout << "Error caught" << endl;
            }
        } else {
            cout << "Error caught" << endl;
        }
    }
    bool attack(Character& target, const string& weaponName) override {
        auto attackerWeapon = arsenal.getContent().front();
        target.hp -= attackerWeapon->healthAffectValue;
        cout << name << " attacks " << target.name << " with their " << weaponName << "!" << endl;
        if (target.hp <= 0) {
            cout << target.name << " has died..." << endl;
            return true;
        }
        return false;
    }
    void show(const string& itemType) override {
        if (itemType == "weapons" || itemType == "weapon") {
            arsenal.showItems(itemType);
        } else if (itemType == "potions" || itemType == "potion") {
            bag.showItems(itemType);
        } else {
            cout << "Error caught" << endl;
        }
    }

};

class Wizard : public Character {
public:
    SpellBook book;
    MedicalBag bag;
    void addItem(const shared_ptr<PhysicalItem>& Item) {
        if (auto spell = dynamic_pointer_cast<Spell>(Item)) {
            if (book.size() < 10) {
                book.addItem(spell);
                cout << name << " just obtained a new spell called " << spell->name << "." << endl;
            } else {
                cout << "Error caught" << endl;
            }
        }
        else if (auto potion = dynamic_pointer_cast<Potion>(Item)) {
            if (bag.size() < 10) {
                bag.addItem(potion);
                cout << name << " just obtained a new potion called " << potion->name << "." << endl;
            } else {
                cout << "Error caught" << endl;
            }
        } else {
            cout << "Error caught" << endl;
        }
    }
    void show(const string& itemType) override {
        if (itemType == "spell" || itemType == "spells") {
            book.showItems(itemType);
        } else if (itemType == "potions" || itemType == "potion") {
            bag.showItems(itemType);
        } else {
            cout << "Error caught" << endl;
        }
    }

};

class Archer : public Character {
public:
    Arsenal arsenal;
    MedicalBag bag;
    SpellBook book;
    void addItem(const shared_ptr<PhysicalItem>& Item) {
        if (auto spell = dynamic_pointer_cast<Spell>(Item)) {
            if (book.size() < 2) {
                book.addItem(spell);
                cout << name << " just obtained a new spell called " << spell->name << "." << endl;
            } else {
                cout << "Error caught" << endl;
            }
        }
        else if (auto potion = dynamic_pointer_cast<Potion>(Item)) {
            if (bag.size() < 3) {
                bag.addItem(potion);
                cout << name << " just obtained a new potion called " << potion->name << "." << endl;
            } else {
                cout << "Error caught" << endl;
            }
        }
        else if (auto weapon = dynamic_pointer_cast<Weapon>(Item)) {
            if (arsenal.size() < 2) {
                arsenal.addItem(weapon);
                cout << name << " just obtained a new weapon called " << weapon->name << "." << endl;
            } else {
                cout << "Error caught" << endl;
            }
        }
    }
    bool attack(Character& target, const string& weaponName) override {
        auto attackerWeapon = arsenal.getContent().front();
        target.hp -= attackerWeapon->healthAffectValue;
        cout << name << " attacks " << target.name << " with their " << weaponName << "!" << endl;
        if (target.hp <= 0) {
            cout << target.name << " has died..." << endl;
            return true;
        }
        return false;
    }
    void show(const string& itemType) override {
        if (itemType == "weapons" || itemType == "weapon") {
            arsenal.showItems(itemType);
        } else if (itemType == "potions" || itemType == "potion") {
            bag.showItems(itemType);
        }else if (itemType == "spells" || itemType == "spell") {
            book.showItems(itemType);
        } else {
            cout << "Error caught" << endl;
        }
    }

};

class Game {
public:
    map<string, shared_ptr<Character>> characters;
    void createCharacter(const string& type, const string& name, int initHP) {
        if (type == "fighter") {
            characters[name] = make_shared<Fighter>();
            cout << "A new fighter came to town, " << name << "." << endl;
        }
        else if (type == "wizard") {
            characters[name] = make_shared<Wizard>();
            cout << "A new wizard came to town, " << name << "." << endl;
        }
        else if (type == "archer") {
            characters[name] = make_shared<Archer>();
            cout << "A new archer came to town, " << name << "." << endl;
        }
        characters[name]->name = name;
        characters[name]->hp = initHP;
    }

    void createItem(const string& itemType, const string& ownerName, const string& itemName, int value, const vector<string>& targetCharacters) {
        auto owner = characters.find(ownerName);
        if (owner != characters.end()) {
            if (itemType == "weapon") {
                auto weapon = make_shared<Weapon>();
                weapon->name = itemName;
                weapon->healthAffectValue = value;
                if (auto fighter = dynamic_pointer_cast<Fighter>(owner->second)) {
                    fighter->addItem(weapon);
                } else if (auto archer = dynamic_pointer_cast<Archer>(owner->second)) {
                    archer->addItem(weapon);
                } else {
                    cout << "Error caught" << endl;
                }
            } else if (itemType == "potion") {
                auto potion = make_shared<Potion>();
                potion->name = itemName;
                potion->healthAffectValue = value;
                if (auto fighter = dynamic_pointer_cast<Fighter>(owner->second)) {
                    fighter->addItem(potion);
                } else if (auto wizard = dynamic_pointer_cast<Wizard>(owner->second)) {
                    wizard->addItem(potion);
                } else if (auto archer = dynamic_pointer_cast<Archer>(owner->second)) {
                    archer->addItem(potion);
                } else {
                    cout << "Error caught" << endl;
                }
            } else if (itemType == "spell" || itemType == "spells") {
                auto spell = make_shared<Spell>();
                spell->name = itemName;
                spell->healthAffectValue = value;
                for (const auto& characterName : targetCharacters) {
                    auto target = characters.find(characterName);
                    if (target != characters.end()) {
                        spell->addOwner(target->second);
                    }
                }
                if (auto wizard = dynamic_pointer_cast<Wizard>(owner->second)) {
                    wizard->addItem(spell);
                } else if (auto archer = dynamic_pointer_cast<Archer>(owner->second)) {
                    archer->addItem(spell);
                } else {
                    cout << "Error caught" << endl;
                }
            } else {
                cout << "Error caught" << endl;
            }
        } else {
            cout << "Error caught" << endl;
        }
    }

    void performAttack(const string& attackerName, const string& targetName, const string& weaponName) {
        auto attacker = characters.find(attackerName);
        auto target = characters.find(targetName);
        if (attacker != characters.end() && target != characters.end()) {
            auto attackerCharacter = attacker->second;
            auto targetCharacter = target->second;
            if (attackerCharacter && targetCharacter) {
                if (auto fighter = dynamic_pointer_cast<Fighter>(attackerCharacter)) {
                    auto attackerArsenal = fighter->arsenal.getContent();
                    auto weapon = find_if(attackerArsenal.begin(), attackerArsenal.end(),
                                          [&](const auto& w) { return w->name == weaponName; });
                    if (weapon != attackerArsenal.end()) {
                        (*fighter).attack(*targetCharacter, weaponName);
                        if (targetCharacter->hp <= 0) {
                            characters.erase(target);
                        }
                    } else {
                        cout << "Error caught" << endl;
                    }
                } else if (auto archer = dynamic_pointer_cast<Archer>(attackerCharacter)) {
                    auto attackerArsenal = archer->arsenal.getContent();
                    auto weapon = find_if(attackerArsenal.begin(), attackerArsenal.end(),
                                          [&](const auto& w) { return w->name == weaponName; });
                    if (weapon != attackerArsenal.end()) {
                        (*archer).attack(*targetCharacter, weaponName);
                        if (targetCharacter->hp <= 0) {
                            characters.erase(target);
                        }
                    } else {
                        cout << "Error caught" << endl;
                    }
                } else {
                    cout << "Error caught" << endl;
                }
            } else {
                cout << "Error caught" << endl;
            }
        } else {
            cout << "Error caught" << endl;
        }
    }

    void performCast(const string& casterName, const string& targetName, const string& spellName) {
        auto caster = characters.find(casterName);
        auto target = characters.find(targetName);
        if (caster != characters.end() && target != characters.end()) {
            auto casterCharacter = caster->second;
            auto targetCharacter = target->second;
            if (auto wizard = dynamic_pointer_cast<Wizard>(casterCharacter)) {
                auto spellBook = wizard->book.getContent();
                auto spell = find_if(spellBook.begin(), spellBook.end(), [&](const auto& s) { return s->name == spellName; });
                if (spell != spellBook.end()) {
                    auto owners = (*spell)->owners;
                    cout << casterName << " casts " << spellName << " on " << targetName << "!" << endl;
                    characters.erase(target);
                    cout << targetName << " has died..." << endl;

                    // Remove the spell from the wizard's spell book
                    wizard->book.getContent().erase(spell);
                } else {
                    cout << "Error caught" << endl;
                }
            } else if (auto archer = dynamic_pointer_cast<Archer>(casterCharacter)) {
                auto spellBook = archer->book.getContent();
                auto spell = find_if(spellBook.begin(), spellBook.end(), [&](const auto& s) { return s->name == spellName; });
                if (spell != spellBook.end()) {
                    auto owners = (*spell)->owners;
                    if (find(owners.begin(), owners.end(), targetCharacter) != owners.end()) {
                        cout << casterName << " casts " << spellName << " on " << targetName << "!" << endl;
                        characters.erase(target);
                        cout << targetName << " has died..." << endl;

                        // Remove the spell from the archer's spell book
                        archer->book.getContent().erase(spell);
                    } else {
                        cout << "Error caught" << endl;
                    }
                } else {
                    cout << "Error caught" << endl;
                }
            } else {
                cout << "Error caught" << endl;
            }
        } else {
            cout << "Error caught" << endl;
        }
    }

    void performDrink(const string& supplierName, const string& drinkerName, const string& potionName) {
        auto supplier = characters.find(supplierName);
        auto drinker = characters.find(drinkerName);
        if (supplier != characters.end() && drinker != characters.end()) {
            cout << drinkerName << " drinks " << potionName << " from " << supplierName << "." << endl;
            auto supplierCharacter = supplier->second;
            if (auto supplierFighter = dynamic_pointer_cast<Fighter>(supplierCharacter)) {
                auto& fighterBag = supplierFighter->bag.getContent();
                if (!fighterBag.empty()) {
                    auto potion = find_if(fighterBag.begin(), fighterBag.end(),
                                          [&](const auto& p) { return p->name == potionName; });
                    if (potion != fighterBag.end()) {
                        drinker->second->hp += (*potion)->healthAffectValue;
                        fighterBag.erase(potion);
                    } else {
                        cout << "Error caught" << endl;
                    }
                } else {
                    cout << "Error caught" << endl;
                }
            } else if (auto supplierWizard = dynamic_pointer_cast<Wizard>(supplierCharacter)) {
                auto& wizardBag = supplierWizard->bag.getContent();
                if (!wizardBag.empty()) {
                    auto potion = find_if(wizardBag.begin(), wizardBag.end(),
                                          [&](const auto& p) { return p->name == potionName; });
                    if (potion != wizardBag.end()) {
                        drinker->second->hp += (*potion)->healthAffectValue;
                        wizardBag.erase(potion);
                    } else {
                        cout << "Error caught" << endl;
                    }
                } else {
                    cout << "Error caught" << endl;
                }
            } else {
                cout << "Error caught" << endl;
            }
        } else {
            cout << "Error caught" << endl;
        }
    }

    void performDialogue(const string& speaker, int sp_len, const vector<string>& speech) {
        if (speaker == "Narrator") {
            cout << "Narrator: ";
            for (const auto& word : speech) {
                cout << word << " ";
            }
            cout << endl;
        }
        else {
            auto character = characters.find(speaker);
            if (character != characters.end()) {
                cout << character->second->name << ": ";
                for (const auto& word : speech) {
                    cout << word << " ";
                }
                cout << endl;
            }
            else {
                cout << "Error caught" << endl;
            }
        }
    }
    static string demangleClassName(const string& mangledName) {
        string demangledName = mangledName;
        while (!demangledName.empty() && (demangledName[0] == '_' || isdigit(demangledName[0]))) {
            demangledName.erase(demangledName.begin());
        }
        transform(demangledName.begin(), demangledName.end(), demangledName.begin(), ::tolower);
        if (!demangledName.empty()) {
            demangledName[0] = tolower(demangledName[0]);
        }
        return demangledName;
    }

    void showCharacters() {
        for (const auto& pair : characters) {
            const auto& character = pair.second;
            string className = demangleClassName(typeid(*character).name());
            cout << character->name << ":" << className << ":" << character->hp << " ";
        }
        cout << endl;
    }

    void showItems(const string& itemType, const string& characterName) {
        auto character = characters.find(characterName);
        if (character != characters.end()) {
            auto characterPtr = character->second;
            if (auto fighter = dynamic_pointer_cast<Fighter>(characterPtr)) {
                fighter->show(itemType);
            } else if (auto wizard = dynamic_pointer_cast<Wizard>(characterPtr)) {
                wizard->show(itemType);
            } else if (auto archer = dynamic_pointer_cast<Archer>(characterPtr)) {
                archer->show(itemType);
            } else {
                cout << "Error caught" << endl;
            }
        } else {
            cout << "Error caught" << endl;
        }
    }


    void processCommands() {
        int n;
        cin >> n;
        if (n < 1 || n > 2000){cout << "Error caught" << endl;}
        cin.ignore();
        for (int i = 0; i < n; ++i) {
            string command;
            getline(cin, command);
            string commandType = command.substr(0, command.find(" "));
            if (commandType == "Create") {
                if (command.find("character") != string::npos) {
                    string characterType = command.substr(command.find("character") + 10, command.find(" ", command.find("character") + 10) - command.find("character") - 10);
                    string rest = command.substr(command.find(" ", command.find("character") + 10) + 1);
                    string name = rest.substr(0, rest.find(" "));
                    rest = rest.substr(rest.find(" ") + 1);
                    int initHP = stoi(rest.substr(0, rest.find(" ")));
                    createCharacter(characterType, name, initHP);
                } else {
                    string itemType = command.substr(command.find("item") + 5, command.find(" ", command.find("item") + 5) - command.find("item") - 5);
                    string rest = command.substr(command.find(" ", command.find("item") + 5) + 1);
                    string ownerName = rest.substr(0, rest.find(" "));
                    rest = rest.substr(rest.find(" ") + 1);
                    string itemName = rest.substr(0, rest.find(" "));
                    rest = rest.substr(rest.find(" ") + 1);
                    int value = stoi(rest.substr(0, rest.find(" ")));
                    rest = rest.substr(rest.find("[") + 1);
                    string targetCharactersString = rest.substr(0, rest.find("]"));
                    vector<string> targetCharacters;
                    size_t pos = 0;
                    string token;
                    while ((pos = targetCharactersString.find(", ")) != string::npos) {
                        token = targetCharactersString.substr(0, pos);
                        targetCharacters.push_back(token);
                        targetCharactersString.erase(0, pos + 2);
                    }
                    targetCharacters.push_back(targetCharactersString);

                    createItem(itemType, ownerName, itemName, value, targetCharacters);
                }
            } else if (commandType == "Attack") {
                string attackerName = command.substr(command.find(" ") + 1, command.find(" ", command.find(" ") + 1) - command.find(" ") - 1);
                string rest = command.substr(command.find(" ", command.find(" ") + 1) + 1);
                string targetName = rest.substr(0, rest.find(" "));
                rest = rest.substr(rest.find(" ") + 1);
                string weaponName = rest;
                performAttack(attackerName, targetName, weaponName);
            } else if (commandType == "Cast") {
                string casterName = command.substr(command.find(" ") + 1, command.find(" ", command.find(" ") + 1) - command.find(" ") - 1);
                string rest = command.substr(command.find(" ", command.find(" ") + 1) + 1);
                string targetName = rest.substr(0, rest.find(" "));
                rest = rest.substr(rest.find(" ") + 1);
                string spellName = rest;
                performCast(casterName, targetName, spellName);
            } else if (commandType == "Drink") {
                string supplierName = command.substr(command.find(" ") + 1, command.find(" ", command.find(" ") + 1) - command.find(" ") - 1);
                string rest = command.substr(command.find(" ", command.find(" ") + 1) + 1);
                string drinkerName = rest.substr(0, rest.find(" "));
                rest = rest.substr(rest.find(" ") + 1);
                string potionName = rest;
                performDrink(supplierName, drinkerName, potionName);
            } else if (commandType == "Dialogue") {
                string speaker = command.substr(command.find(" ") + 1, command.find(" ", command.find(" ") + 1) - command.find(" ") - 1);
                string rest = command.substr(command.find(" ", command.find(" ") + 1) + 1);
                int sp_len = stoi(rest.substr(0, rest.find(" ")));
                rest = rest.substr(rest.find(" ") + 1);
                vector<string> speech;
                for (int j = 0; j < sp_len; ++j) {
                    if (j == sp_len - 1) {
                        speech.push_back(rest);
                    } else {
                        speech.push_back(rest.substr(0, rest.find(" ")));
                        rest = rest.substr(rest.find(" ") + 1);
                    }
                }
                performDialogue(speaker, sp_len, speech);
            } else if (commandType == "Show") {
                if (command.find("characters") != string::npos) {
                    showCharacters();
                } else {
                    string itemType = command.substr(command.find("items") + 6, command.find(" ", command.find("items") + 6) - command.find("items") - 6);
                    string characterName = command.substr(command.find(" ", command.find("items") + 6) + 1);
                    showItems(itemType, characterName);
                }
            }
        }
    }
};


int main() {
    Game game;
    Narrator& narrator = Narrator::getInstance();
    ofstream outputFile("output.txt");
    if (!outputFile.is_open()) {
        cerr << "Error: Unable to open output file." << endl;
        return 1;
    }
    streambuf *coutbuf = cout.rdbuf();
    cout.rdbuf(outputFile.rdbuf());
    ifstream inputFile("input.txt");
    if (!inputFile.is_open()) {
        cerr << "Error: Unable to open input file." << endl;
        return 1;
    }
    streambuf *cinbuf = cin.rdbuf();
    cin.rdbuf(inputFile.rdbuf());
    game.processCommands();
    inputFile.close();
    cin.rdbuf(cinbuf);
    cout.rdbuf(coutbuf);
    outputFile.close();
    return 0;
}
