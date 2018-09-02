#include "NPCHero.h"
#include "NPCSkillPack.h"

NPCHero::NPCHero(QObject *parent) : QObject(parent)
{
    attribute1();
    attribute2();
    attribute3();
    attribute4();
    attribute5();
}

QString NPCHero::name() const
{
    return m_name;
}

QString NPCHero::origin() const
{
    return m_origin;
}

QString NPCHero::profession() const
{
    return m_profession;
}

QString NPCHero::specialization() const
{
    return m_specialization;
}

QString NPCHero::sickness() const
{
    return m_sickness;
}

QString NPCHero::feature1() const
{
    return m_feature1;
}

QString NPCHero::feature2() const
{
    return m_feature2;
}

int NPCHero::reputation() const
{
    return m_reputation;
}

int NPCHero::fame() const
{
    return m_fame;
}

NPCAttribute *NPCHero::attribute(const QString &name) const
{
    NPCAttribute *pAttribute{nullptr};

    for ( NPCAttribute* tAttribute: m_attributes )
        if ( tAttribute->name() == name ) {
            pAttribute = tAttribute;
            break;
        }

    return pAttribute;
}

QVector<NPCAttribute *> NPCHero::attributes() const
{
    return m_attributes;
}

void NPCHero::setName(const QString &name)
{
    m_name = name;
}

void NPCHero::setOrigin(const QString &origin)
{
    m_origin = origin;
}

void NPCHero::setProfession(const QString &profession)
{
    m_profession = profession;
}

void NPCHero::setSpecialization(const QString &specialization)
{
    m_specialization = specialization;
}

void NPCHero::setSickness(const QString &sickness)
{
    m_sickness = sickness;
}

void NPCHero::setFeature1(const QString &feature)
{
    m_feature1 = feature;
}

void NPCHero::setFeature2(const QString &feature)
{
    m_feature2 = feature;
}

void NPCHero::setReputation(const int &reputation)
{
    m_reputation = reputation;
}

void NPCHero::setFame(const int &fame)
{
    m_fame = fame;
}

void NPCHero::increaseAttribute(const QString &name, const int &value)
{
    for ( NPCAttribute* tAttribute: m_attributes )
        if ( tAttribute->name() == name ) {
            tAttribute->increaseValue( value );
            break;
        }
}

void NPCHero::setAttributeModificator(const QString &name, const int &value)
{
    for ( NPCAttribute* tAttribute: m_attributes )
        if ( tAttribute->name() == name ) {
            tAttribute->setModificator( value );
            break;
        }
}

void NPCHero::addTrick(const QString &trick)
{
    m_tricks.push_back( trick );
}

void NPCHero::attribute1()
{
    NPCAttribute *pAttribute = new NPCAttribute("Budowa", 0, 0);

    NPCSkillPack *pSkillPack1 = new NPCSkillPack( "Sprawność",
                                                  QStringList{"Ranger"},
                                                  QStringList{"Kondycja",
                                                              "Pływanie",
                                                              "Wspinaczka"} );
    NPCSkillPack *pSkillPack2 = new NPCSkillPack( "Jeździectwo",
                                                  QStringList{"Ranger"},
                                                  QStringList{"Jazda konna",
                                                              "Powożenie",
                                                              "Ujeżdżanie"} );

    pAttribute->addSkillPack( pSkillPack1 );
    pAttribute->addSkillPack( pSkillPack2 );

    m_attributes.push_back( pAttribute );
}

void NPCHero::attribute2()
{
    NPCAttribute *pAttribute = new NPCAttribute("Zręczność", 0, 0);

    NPCSkillPack *pSkillPack1 = new NPCSkillPack( "Walka Wręcz",
                                                  QStringList{"Wojownik"},
                                                  QStringList{"Bijatyka",
                                                              "Broń ręczna",
                                                              "Rzucanie"} );
    NPCSkillPack *pSkillPack2 = new NPCSkillPack( "Prowadzenie Pojazdów",
                                                  QStringList{"Technik"},
                                                  QStringList{"Samochód",
                                                              "Motocykl",
                                                              "Ciężarówka"} );
    NPCSkillPack *pSkillPack3 = new NPCSkillPack( "Zdolności Manualne",
                                                  QStringList{"Cwaniak"},
                                                  QStringList{"Kradzież kieszonkowa",
                                                              "Otwieranie zamków",
                                                              "Zwinne dłonie"} );
    NPCSkillPack *pSkillPack4 = new NPCSkillPack( "Broń Strzelecka",
                                                  QStringList{"Wojownik"},
                                                  QStringList{"Pistolety",
                                                              "Karabiny",
                                                              "Broń maszynowa"} );
    NPCSkillPack *pSkillPack5 = new NPCSkillPack( "Broń Dystansowa",
                                                  QStringList{"Ranger", "Wojownik"},
                                                  QStringList{"Łuk",
                                                              "Kusza",
                                                              "Proca"} );

    pAttribute->addSkillPack( pSkillPack1 );
    pAttribute->addSkillPack( pSkillPack2 );
    pAttribute->addSkillPack( pSkillPack3 );
    pAttribute->addSkillPack( pSkillPack4 );
    pAttribute->addSkillPack( pSkillPack5 );

    m_attributes.push_back( pAttribute );
}

void NPCHero::attribute3()
{
    NPCAttribute *pAttribute = new NPCAttribute("Charakter", 0, 0);

    NPCSkillPack *pSkillPack1 = new NPCSkillPack( "Negocjacje",
                                                  QStringList{"Cwaniak"},
                                                  QStringList{"Zastraszanie",
                                                              "Perswazja",
                                                              "Zdolności przywódcze"} );
    NPCSkillPack *pSkillPack2 = new NPCSkillPack( "Empatia",
                                                  QStringList{"Cwaniak"},
                                                  QStringList{"Postrzeganie emocji",
                                                              "Blef",
                                                              "Opieka nad zwierzętami"} );
    NPCSkillPack *pSkillPack3 = new NPCSkillPack( "Siła Woli",
                                                  QStringList{"Wojownik"},
                                                  QStringList{"Odporność na ból",
                                                              "Niezłomność",
                                                              "Morale"} );

    pAttribute->addSkillPack( pSkillPack1 );
    pAttribute->addSkillPack( pSkillPack2 );
    pAttribute->addSkillPack( pSkillPack3 );

    m_attributes.push_back( pAttribute );
}

void NPCHero::attribute4()
{
    NPCAttribute *pAttribute = new NPCAttribute("Spryt", 0, 0);

    NPCSkillPack *pSkillPack1 = new NPCSkillPack( "Medycyna",
                                                  QStringList{"Ranger", "Technik"},
                                                  QStringList{"Pierwsza pomoc",
                                                              "Leczenie ran",
                                                              "Leczenie chorób"} );
    NPCSkillPack *pSkillPack2 = new NPCSkillPack( "Technika",
                                                  QStringList{"Technik"},
                                                  QStringList{"Mechanika",
                                                              "Elektronika",
                                                              "Komputery"} );
    NPCSkillPack *pSkillPack3 = new NPCSkillPack( "Wiedza ogólna",
                                                  QStringList{"Technik"},
                                                  QStringList{"Historia",
                                                              "Biologia",
                                                              "Chemia"} );
    NPCSkillPack *pSkillPack4 = new NPCSkillPack( "Sprzęt",
                                                  QStringList{"Technik"},
                                                  QStringList{"Maszyny ciężkie",
                                                              "Wozy bojowe",
                                                              "Kutry"} );
    NPCSkillPack *pSkillPack5 = new NPCSkillPack( "Pirotechnika",
                                                  QStringList{"Wojownik", "Technik"},
                                                  QStringList{"Rusznikarstwo",
                                                              "Wyrzutnie",
                                                              "Materiały wybuchowe"} );

    pAttribute->addSkillPack( pSkillPack1 );
    pAttribute->addSkillPack( pSkillPack2 );
    pAttribute->addSkillPack( pSkillPack3 );
    pAttribute->addSkillPack( pSkillPack4 );
    pAttribute->addSkillPack( pSkillPack5 );

    m_attributes.push_back( pAttribute );
}

void NPCHero::attribute5()
{
    NPCAttribute *pAttribute = new NPCAttribute("Percepcja", 0, 0);

    NPCSkillPack *pSkillPack1 = new NPCSkillPack( "Orientacja w Terenie",
                                                  QStringList{"Ranger"},
                                                  QStringList{"Wyczucie kierunku",
                                                              "Przygotowanie pułapki",
                                                              "Tropienie"} );
    NPCSkillPack *pSkillPack2 = new NPCSkillPack( "Spostrzegawczość",
                                                  QStringList{"Ranger"},
                                                  QStringList{"Nasłuchiwanie",
                                                              "Wypatrywanie",
                                                              "Czujność"} );
    NPCSkillPack *pSkillPack3 = new NPCSkillPack( "Kamuflaż",
                                                  QStringList{"Ranger", "Cwaniak"},
                                                  QStringList{"Skradanie się",
                                                              "Ukrywanie się",
                                                              "Maskowanie"} );
    NPCSkillPack *pSkillPack4 = new NPCSkillPack( "Przetrwanie",
                                                  QStringList{"Ranger"},
                                                  QStringList{"Łowiectwo",
                                                              "Znajomość terenu",
                                                              "Zdobywanie wody"} );

    pAttribute->addSkillPack( pSkillPack1 );
    pAttribute->addSkillPack( pSkillPack2 );
    pAttribute->addSkillPack( pSkillPack3 );
    pAttribute->addSkillPack( pSkillPack4 );

    m_attributes.push_back( pAttribute );
}
