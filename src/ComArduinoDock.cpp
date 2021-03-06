#include "ComArduinoDock.h"
#include <QTranslator>
#include <QtCore>
#include "qextserialport.h"
#include "qextserialenumerator.h"
#include <QMessageBox>
#include <QColor>

ComArduino::ComArduino(const QString & title, QWidget *parent , Qt::WindowFlags flags) :
    QDockWidget(title, parent, flags)
{
#ifdef DEBUG_COMANDSAVE
    std::cout << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
    //Initialisations des widgets composant le dock
    //Définition d'une police pour le dock
    QFont font("Verdana", 9);
    setFont(font);
    //Tab du Dock
    TabDock = new QTabWidget;
    TabDock->setMaximumWidth(500);
    TabDock->setMinimumWidth(300);
    TabDock->setContentsMargins(1,1,1,1);
    TabDock->setStyleSheet("QTabBar::tab{height:20px}");
    TabDock->setFont(font);
    //Widget de base pour accueillir le layout Principale
    Base = new QWidget;
    //Widget qui contient la partie liaison Arduino
    LiaisonArduino = new QWidget;
    //Widget qui permet l'échange de donnée avec Arduino
    Parametre = new QWidget;
    //Widget qui accueil le layout avec la zone pour la console
    TerminalZone = new QWidget;
    //Label pour afficher la date et l'heure du PC
    LabelDateEtHeure = new QLabel;
    LabelDateEtHeure->setText(tr("Date<br>Heure"));
    LabelDateEtHeure->setFont(font);
    //Label Baude Rate
    LabelBaudeRate = new QLabel(tr("Baude Rate"));
    LabelBaudeRate->setFont(font);
    //Label Parity
    LabelParity = new QLabel(tr("Parity"));
    LabelParity->setFont(font);
    //Label DataBit
    LabelDataBit = new QLabel(tr("Data Bit"));
    LabelDataBit->setFont(font);
    //Label Stop Bit
    LabelStopBit = new QLabel(tr("Stop Bit"));
    LabelStopBit->setFont(font);
    //Label Query mode
    LabelQueryMode = new QLabel(tr("Query Mode"));
    LabelQueryMode->setFont(font);
    //Label ports
    LabelPort = new QLabel(tr("Liste des ports"));
    LabelPort->setFont(font);
    //Label time out
    LabelTimeOut = new QLabel(tr("Time Out"));
    LabelTimeOut->setFont(font);
    //Label Module
    LabelModule = new QLabel(tr("Module destinataire"));
    LabelModule->setFont(font);
    //Label type de commande
    LabelTypeCmd = new QLabel(tr("Type d'instruction"));
    LabelTypeCmd->setFont(font);
    LabelTypeCmd->setHidden(true);
    //Label Commande
    LabelCmd = new QLabel(tr("Instruction"));
    LabelCmd->setFont(font);
    LabelCmd->setHidden(true);
    //Label instruction
    LabelInstruction = new QLabel(tr("Paramètre complémentaire"));
    LabelInstruction->setFont(font);
    LabelInstruction->setHidden(true);
    //Label aide pour le parametre
    LabelAideParametre = new QLabel;
    LabelAideParametre->setFont(font);
    LabelAideParametre->setHidden(true);
    //Insertion de l'icone déco dans le layout IconeElectronique via un label
    label_img = new QLabel;
    QPixmap Icone(":Arduino");
    label_img->setPixmap(Icone);
    label_img->setMaximumWidth(120);
    //Layout Horizontale pour équilibrer le dock
    LayoutPrincH = new QHBoxLayout;
    LayoutPrincH->setContentsMargins(1,1,1,1);
    LayoutPrincH->setSpacing(1);
    //Layout du 1er onglet
    EspaceDockLiaison = new QHBoxLayout;
    EspaceDockLiaison->setContentsMargins(1,1,1,1);
    EspaceDockLiaison->setSpacing(1);
    //Layout V pour placer les boutons de communication
    LayoutEnvoisRecep = new QVBoxLayout;
    LayoutEnvoisRecep->setSpacing(1);
    LayoutEnvoisRecep->setContentsMargins(1,1,1,1);
    LayoutEnvoisRecep->setAlignment(Qt::AlignTop);
    //Layout Horizontale pour l'affichage de la date, l'heure
    DateTimeConnect = new QVBoxLayout;
    DateTimeConnect->setAlignment(Qt::AlignVCenter);
    DateTimeConnect->setSpacing(1);
    DateTimeConnect->setContentsMargins(1,1,1,1);
    //Layout pour placer le terminal et la zone de commande manuelle
    EspaceTerminal = new QVBoxLayout;
    EspaceTerminal->setContentsMargins(1,1,1,1);
    EspaceTerminal->setSpacing(1);
    //Layout H de commande manuel de l'arduino
    LayoutCommandeMan = new QHBoxLayout;
    LayoutCommandeMan->setSpacing(1);
    //Layout pour accueillir les paramètre de connection Partie droite
    LayoutParamD = new QVBoxLayout;
    LayoutParamD->setContentsMargins(1,1,1,1);
    LayoutParamD->setSpacing(1);
    //Layout pour accueillir les paramètre de connextion Partie Gauche
    LayoutParamG = new QVBoxLayout;
    LayoutParamG->setContentsMargins(1,1,1,1);
    LayoutParamG->setSpacing(1);
    //Layout horizontale pour l'espace paramètre
    EspaceParam = new QHBoxLayout;
    EspaceParam->setContentsMargins(1,1,1,1);
    EspaceParam->setSpacing(1);
    //Led Vert pour la connection
    LedVerte = new HLed;
    LedVerte->turnOff();
    LedVerte->setFixedHeight(22);
    LedVerte->setFixedWidth(22);
    //Terminal de réception USBSériale
    TerminalUSBSerial = new QPlainTextEdit;
    TerminalUSBSerial->insertPlainText(Const_QString(0));
    TerminalUSBSerial->setReadOnly(true);
    TerminalUSBSerial->setFont(font);
    //Boutton pour envoyer a l'arduino une instruction
    ButtonSend = new QPushButton(tr("Envoyer"));
    ButtonSend->setMinimumWidth(50);
    ButtonSend->setFixedHeight(22);
    ButtonSend->setFont(font);
    //Ligne d'édition pour introduire une commande manuelle
    InstructionManuel = new QLineEdit;
    InstructionManuel->setFixedHeight(22);
    InstructionManuel->setFont(font);
    //Ligne d'édition pour instroduire les paramètres complémentaires
    LineEditInstruction = new QLineEdit;
    LineEditInstruction->setFont(font);
    LineEditInstruction->setFixedHeight(22);
    LineEditInstruction->setHidden(true);
    //ComboBox Baude Rate
    BaudeRate = new QComboBox;
    BaudeRate->setFont(font);
    BaudeRate->addItem("1200", BAUD1200);
    BaudeRate->addItem("2400", BAUD2400);
    BaudeRate->addItem("4800", BAUD4800);
    BaudeRate->addItem("9600", BAUD9600);
    BaudeRate->addItem("19200", BAUD19200);
    BaudeRate->setCurrentIndex(3);
    //ComboBox Parity Box
    ParityBox = new QComboBox;
    ParityBox->setFont(font);
    ParityBox->addItem("NONE", PAR_NONE);
    ParityBox->addItem("ODD", PAR_ODD);
    ParityBox->addItem("EVEN", PAR_EVEN);
    //ComboBox Databits Box
    DataBitBox = new QComboBox;
    DataBitBox->addItem("5", DATA_5);
    DataBitBox->addItem("6", DATA_6);
    DataBitBox->addItem("7", DATA_7);
    DataBitBox->addItem("8", DATA_8);
    DataBitBox->setCurrentIndex(3);
    //ComboBox Stop Bit Box
    StopBitBox = new QComboBox;
    StopBitBox->setFont(font);
    StopBitBox->addItem("1", STOP_1);
    StopBitBox->addItem("2", STOP_2);
    //ComboBox Query Mode Box
    QueryModeBox = new QComboBox;
    QueryModeBox->setFont(font);
    QueryModeBox->addItem(tr("Polling","Méthode utilisée"), QextSerialPort::Polling);
    QueryModeBox->addItem(tr("EventDriven","Méthode Utilisée"), QextSerialPort::EventDriven);
    //Spin Box time Out Box
    TimeOutBox = new QSpinBox;
    TimeOutBox->setFont(font);
    TimeOutBox->setSuffix("ms");
    TimeOutBox->setMaximum(1000);
    TimeOutBox->setMinimum(0);
    TimeOutBox->setValue(10);
    TimeOutBox->setSingleStep(10);
    //ComboBox selection Module
    QComboModul = new QComboBox;
    QComboModul->setFont(font);
    QComboModul->addItem("");
    //ComboBox selection Type de commande
    QComboTypeCmd = new QComboBox;
    QComboTypeCmd->setFont(font);
    QComboTypeCmd->setHidden(true);
    QComboTypeCmd->addItem("");
    //ComboBox selection de commande
    QComboCmd = new QComboBox;
    QComboCmd->setFont(font);
    QComboCmd->setHidden(true);
    QComboCmd->addItem("");
    //ComboBox Port Box list
    PortBox = new QComboBox;
    PortBox->setFont(font);
    //Fenetre convertisseur Décimal vers Hexadécimal
    Convert = new ConvertisseurDecHexa;
    Convert->setHidden(true);

    //Dessin du dock en attribuant les références de parents enfants
             //Ajout du Label avec la date et l'heure
             DateTimeConnect->addWidget(LabelDateEtHeure);
             //Ajout de l'icone électronique
             DateTimeConnect->addWidget(label_img,0,Qt::AlignCenter);
             //Ajout du comboBox de sélection du port et précédé de son label
             DateTimeConnect->addWidget(LabelPort);
             DateTimeConnect->addWidget(PortBox);
          //Ajout de la zone Date et Heure
          EspaceDockLiaison->addLayout(DateTimeConnect);
             //Ajout du labal module
             LayoutEnvoisRecep->addWidget(LabelModule);
             //Ajout du combobox module
             LayoutEnvoisRecep->addWidget(QComboModul);
             //Ajout du label type de commande
             LayoutEnvoisRecep->addWidget(LabelTypeCmd);
             //Ajout du combobox type de commande
             LayoutEnvoisRecep->addWidget(QComboTypeCmd);
             //Ajout du label commande
             LayoutEnvoisRecep->addWidget(LabelCmd);
             //ajout du combobox commande
             LayoutEnvoisRecep->addWidget(QComboCmd);
             //Ajout du label Instruction
             LayoutEnvoisRecep->addWidget(LabelInstruction);
             //Ajout du QligneEdit pour les paramètre
             LayoutEnvoisRecep->addWidget(LineEditInstruction);
             //Ajout du Label d'aide pour les paramètre
             LayoutEnvoisRecep->addWidget(LabelAideParametre);
             //Ajout du convertisseur hexa -> decimal
             LayoutEnvoisRecep->addWidget(Convert);
          //Ajout de la zone envois recep Dans l'espace DockLiaison
          EspaceDockLiaison->addLayout(LayoutEnvoisRecep);
       //Application du layout au widget liaison Arduino
       LiaisonArduino->setLayout(EspaceDockLiaison);
    //Création d'un ongle avec le widget Liaison Arduino
    TabDock->addTab(LiaisonArduino, tr("Envoi/Reception"));
    TabDock->setTabToolTip(0,tr("Envois et réception de donnée avec le module Arduino"));
             //Ajout du comboBox Baude Rate Au menu param gauche et précédé de son label
             LayoutParamG->addWidget(LabelBaudeRate);
             LayoutParamG->addWidget(BaudeRate);
             //Ajout du comboBox Parity Box au menu param gauche et précédé de son label
             LayoutParamG->addWidget(LabelParity);
             LayoutParamG->addWidget(ParityBox);
             //Ajout du comboBox StopBit au menu param gauche et précédé de son label
             LayoutParamG->addWidget(LabelStopBit);
             LayoutParamG->addWidget(StopBitBox);
             //Ajout du comboBox DataBit Box au menu param droite et précédé de son label
             LayoutParamD->addWidget(LabelDataBit);
             LayoutParamD->addWidget(DataBitBox);
             //Ajout du comboBox QueryMode au menu param droite et précédé de son label
             LayoutParamD->addWidget(LabelQueryMode);
             LayoutParamD->addWidget(QueryModeBox);
             //Ajout du spinBox TimeOut au menu param droite et précédé de son label
             LayoutParamD->addWidget(LabelTimeOut);
             LayoutParamD->addWidget(TimeOutBox);
          //Application du layout param gauche au layout param et précédé de son label
          EspaceParam->addLayout(LayoutParamG);
          //Application du layout param droite au layout param et précédé de son label
          EspaceParam->addLayout(LayoutParamD);
       //Application du layout param au widget param
       Parametre->setLayout(EspaceParam);
    //Création d'un ongle avec le widget Envois Recept
    TabDock->addTab(Parametre, tr("Paramètres"));
    TabDock->setTabToolTip(1,tr("Outil pour s'assurer que le PC communique bien avec l'Arduino"));
    //Ajout dans le Layout Principale du tab
    LayoutPrincH->addWidget(TabDock);
          //Ajout de la Led verte de connection
          LayoutCommandeMan->addWidget(LedVerte);
          //Ajout de la ligne d'instruction dans le layout H
          LayoutCommandeMan->addWidget(InstructionManuel);
          //Ajout du boutton d'envois a coté de la ligne d'instruction
          LayoutCommandeMan->addWidget(ButtonSend);
       //Ajout du plainTextEdit dans le layout de l'espace Terminal
       EspaceTerminal->addWidget(TerminalUSBSerial);
       //Ajout de la zone de commande manuel dans l'espace terminal
       EspaceTerminal->addLayout(LayoutCommandeMan);
       //Application du layout de l'EspaceTerminal dans le widget terminal
       TerminalZone->setLayout(EspaceTerminal);
    //Ajout dans le layout principale
    LayoutPrincH->addWidget(TerminalZone);
    //Application du layout Principale dans le widget de base
    Base->setLayout(LayoutPrincH);
    //Application du layout Principale au dock
    setWidget(Base);
    //Zone autorisée pour le dock
    setAllowedAreas(Qt::BottomDockWidgetArea);

    timer = new QTimer(this);
    timer->setInterval(40);

    //Timer qui déclanche un signal pour rafraichir l'heure 1 fois par seconde
    ClockUpdate = new QTimer(this);
    ClockUpdate->setInterval(1000);
    ClockUpdate->start();
    connect(ClockUpdate, SIGNAL(timeout()), this, SLOT(onTimeUpdate()));

    //Variable pour l'affichage de la date et l'heure
    dateEtHeure = new QDateTime();

    //Création du port usbsérie
    PortSettings settings = {BAUD9600, DATA_8, PAR_NONE, STOP_1, FLOW_OFF, 10};
    port = new QextSerialPort(PortBox->currentText(), settings, QextSerialPort::Polling);

    //Liste des port usbsérie disponible
    enumerator = new QextSerialEnumerator(this);
    enumerator->setUpNotifications();

    //Création l'arbre Xml avec la liste des commandes que l'on peut envoyer aux modules
    docCommandeModule = new QDomDocument();
    QFile *file = new QFile(":cmd.xml");
#ifdef DEBUG_COMANDSAVE
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, tr("Erreur a l'ouverture du fichier"),tr("Une erreur à l'ouverture du fichier \"CommandeModule.xml\"."),QMessageBox::Ok);
    }
    if (!docCommandeModule->setContent(file)) {
        QMessageBox::critical(this, tr("Erreur a la lecture du fichier"),tr("Une erreur à la lecture du fichier \"CommandeModule.xml\"."),QMessageBox::Ok);
    }
#else
    file->open(QIODevice::ReadOnly | QIODevice::Text);
    docCommandeModule->setContent(file);
#endif /* DEBUG_COMANDSAVE */
    file->close();
    delete file;
    //On peuple directement la comboboxModule
    QDomElement element = docCommandeModule->documentElement();
    for(QDomElement qde = element.firstChildElement(); !qde.isNull(); qde = qde.nextSiblingElement())
    {
        //Prend le nom et vérifie l'initialisation du mode
        if (qde.tagName() == "Module")
        {
            QComboModul->addItem(qde.attribute("Nom"), qde.attribute("Cmd"));
        }
    }

    //Aplication d'un style au dock
    setStyleSheet("QDockWidget::title {text-align: centre; background: rgb(0,189,138); color: rgb(0,0,138); border-style: inset; border-width: 2px; border-radius: 2px; border-color: rgb(0,0,138); font: 8pt; padding: 0px; height: 20px} QDockWidget::float-button { border: 1px solid; background: rgb(0,189,138); padding: 0px} QDockWidget::float-button:hover { background: rgb(0,0,138) } QDockWidget {border-style: outset; border-width: 2px; border-radius: 4px; border-color: rgb(0,0,138)}");
    setFeatures(QDockWidget::NoDockWidgetFeatures);
    setFeatures(QDockWidget::DockWidgetMovable);
    setFeatures(QDockWidget::DockWidgetFloatable);

    //Connection des actionneurs divers au différentes actions
    //Paramettrage du port serie
    connect(BaudeRate, SIGNAL(currentIndexChanged(int)), SLOT(onBaudRateChanged(int)));
    connect(ParityBox, SIGNAL(currentIndexChanged(int)),SLOT(onParityChanged(int)));
    connect(DataBitBox, SIGNAL(currentIndexChanged(int)), SLOT(onDataBitsChanged(int)));
    connect(StopBitBox, SIGNAL(currentIndexChanged(int)), SLOT(onStopBitsChanged(int)));
    connect(QueryModeBox, SIGNAL(currentIndexChanged(int)), SLOT(onQueryModeChanged(int)));
    connect(TimeOutBox, SIGNAL(valueChanged(int)), SLOT(onTimeoutChanged(int)));
    connect(PortBox, SIGNAL(currentIndexChanged(QString)), SLOT(onPortNameChanged(QString)));
    //Envoi de la commande introduite dans le champs par l'utilisateur
    connect(ButtonSend, SIGNAL(clicked()), SLOT(onSendButtonClicked()));
    //Réception de donnée sur le port série
    connect(timer, SIGNAL(timeout()), SLOT(onReadyRead()));
    connect(port, SIGNAL(readyRead()), SLOT(onReadyRead()));
    //Signal pour lancer l'ouverture ou la fermeture du port série
    //Il est possible de mettre un bouton qui agit sur le meme slot.
    //Inutile ici, car le signal est générer automatiquement au branchement du cable USB
    connect(this, SIGNAL(genereOpenCloseEvent()), SLOT(onOpenCloseButtonClicked()));
    //Signaux pour la gestion des combobox pour envoyer des messages système vers les modules arduino
    connect(QComboModul, SIGNAL(currentIndexChanged(int)), SLOT(onModuleChanged(int)));
    connect(QComboTypeCmd, SIGNAL(currentIndexChanged(int)), SLOT(onTypeCmdChanged(int)));
    connect(QComboCmd, SIGNAL(currentIndexChanged(int)), SLOT(onCmdChanged(int)));
    connect(Convert, SIGNAL(HexaVal(QString)), SLOT(onHexaCame(QString)));
    //Signal return pressed des zones d'édition
    connect(InstructionManuel, SIGNAL(returnPressed()), SLOT(onSendButtonClicked()));
    connect(LineEditInstruction, SIGNAL(returnPressed()), SLOT(onSendButtonClicked()));

#ifdef DEBUG_COMANDSAVE
    std::cout << "/" << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
}

ComArduino::~ComArduino()
{
#ifdef DEBUG_COMANDSAVE
    std::cout << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
    Convert->close();
    delete port;
    delete Convert;
    delete LabelModule;
    delete LabelTypeCmd;
    delete LabelCmd;
    delete LabelInstruction;
    delete QComboModul;
    delete QComboCmd;
    delete QComboTypeCmd;
    delete LineEditInstruction;
    delete LedVerte;
    delete LabelDateEtHeure;
    delete LabelBaudeRate;
    delete LabelParity;
    delete LabelDataBit;
    delete LabelStopBit;
    delete LabelQueryMode;
    delete LabelPort;
    delete LabelTimeOut;
    delete label_img;
    delete TerminalUSBSerial;
    delete ButtonSend;
    delete InstructionManuel;
    delete BaudeRate;
    delete ParityBox;
    delete DataBitBox;
    delete StopBitBox;
    delete QueryModeBox;
    delete TimeOutBox;
    delete PortBox;
    delete LayoutParamD;
    delete LayoutParamG;
    delete EspaceParam;
    delete Parametre;
    delete LayoutCommandeMan;
    delete LayoutEnvoisRecep;
    delete DateTimeConnect;
    delete EspaceDockLiaison;
    delete LiaisonArduino;
    delete TabDock;
    delete EspaceTerminal;
    delete TerminalZone;
    delete LayoutPrincH;
    delete Base;
    delete docCommandeModule;

#ifdef DEBUG_COMANDSAVE
    std::cout << "/" << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
}

//Fonction de mise à jour de l'heure
void ComArduino::onTimeUpdate()
{
#ifdef DEBUG_COMANDSAVE
    std::cout << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
    QFont font("Verdana", 9);
    dateEtHeure->setDate(QDate::currentDate());
    dateEtHeure->setTime(QTime::currentTime());
    LabelDateEtHeure->setText(dateEtHeure->toString(tr("ddd dd MMM yyyy<br>hh:mm:ss ap","Date et Heure en mode 12h (am/pm), Exemple: Sam. 24 Aout 2013 (a la ligne) 10:25:14 am")));
    LabelDateEtHeure->setFont(font);
    ClockUpdate->start();
    //On  en profite pour mettre la liste des ports a jour. Permet de réaliser dans la seconde
    //une connextion avec un module qui vient d'etre branché. -> Suppression du bouton Open/Close
    onPortAddedOrRemoved();
    if (!port->isOpen() && PortBox->count() > 0) onOpenCloseButtonClicked(OPEN_PORT);
    if (port->isOpen() && PortBox->count() == 0) onOpenCloseButtonClicked(CLOSE_PORT);
#ifdef DEBUG_COMANDSAVE
    std::cout << "/" << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
}

void ComArduino::onPortNameChanged(const QString & )
{
#ifdef DEBUG_COMANDSAVE
    std::cout << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
    if (port->isOpen()) {
        emit genereOpenCloseEvent();
    }
#ifdef DEBUG_COMANDSAVE
    std::cout << "/" << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
}

void ComArduino::onBaudRateChanged(int idx)
{
#ifdef DEBUG_COMANDSAVE
    std::cout << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
    port->setBaudRate((BaudRateType)BaudeRate->itemData(idx).toInt());
#ifdef DEBUG_COMANDSAVE
    std::cout << "/" << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
}

void ComArduino::onParityChanged(int idx)
{
#ifdef DEBUG_COMANDSAVE
    std::cout << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
    port->setParity((ParityType)ParityBox->itemData(idx).toInt());
#ifdef DEBUG_COMANDSAVE
    std::cout << "/" << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
}

void ComArduino::onDataBitsChanged(int idx)
{
#ifdef DEBUG_COMANDSAVE
    std::cout << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
    port->setDataBits((DataBitsType)DataBitBox->itemData(idx).toInt());
#ifdef DEBUG_COMANDSAVE
    std::cout << "/" << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
}

void ComArduino::onStopBitsChanged(int idx)
{
#ifdef DEBUG_COMANDSAVE
    std::cout << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
    port->setStopBits((StopBitsType)StopBitBox->itemData(idx).toInt());
#ifdef DEBUG_COMANDSAVE
    std::cout << "/" << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
}


void ComArduino::onQueryModeChanged(int idx)
{
#ifdef DEBUG_COMANDSAVE
    std::cout << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
    port->setQueryMode((QextSerialPort::QueryMode)QueryModeBox->itemData(idx).toInt());
#ifdef DEBUG_COMANDSAVE
    std::cout << "/" << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
}

void ComArduino::onTimeoutChanged(int val)
{
#ifdef DEBUG_COMANDSAVE
    std::cout << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
    port->setTimeout(val);
#ifdef DEBUG_COMANDSAVE
    std::cout << "/" << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
}

void ComArduino::onOpenCloseButtonClicked()
{
#ifdef DEBUG_COMANDSAVE
    std::cout << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
    onOpenCloseButtonClicked(0);
#ifdef DEBUG_COMANDSAVE
    std::cout << "/" << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
}

void ComArduino::onOpenCloseButtonClicked(char action)
{
#ifdef DEBUG_COMANDSAVE
    std::cout << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
    if (PortBox->currentText().contains("ttyACM"))
    {
        if (!port->isOpen())
        {
            if (action == OPEN_PORT || action == 0)
            {
                port->setPortName(PortBox->currentText());
                port->open(QIODevice::ReadWrite);
            }
        }
        else
        {
            if (action == CLOSE_PORT || action == 0)
            {
                port->close();
            }
        }
    }
    else
    {
        port->close();
    }

    //If using polling mode, we need a QTimer
    if (port->isOpen() && port->queryMode() == QextSerialPort::Polling)
        timer->start();
    else
        timer->stop();

    //update led's status
    LedVerte->turnOn(port->isOpen());

#ifdef DEBUG_COMANDSAVE
    std::cout << "/" << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
}

//Fonction qui envois des données sur le port série
void ComArduino::onSendButtonClicked(QByteArray commandeToSend)
{
#ifdef DEBUG_COMANDSAVE
    std::cout << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
    //1er cas on envois le message introduit par l'utilisateur
    if (port->isOpen() && commandeToSend == "")
    {
        //Si il n'y a pas d'instruction manuel et pas de commandeToSend alors on regarde les messages système
        //Le message doit être composé a partir des différentes sélection faite par l'utilisateur
        if (InstructionManuel->text().isEmpty() && QComboModul->currentIndex() != 0) {
            QString Message = "<";
            Message.append(QComboModul->currentData().toString());
            Message.append("M00");
            Message.append(QComboTypeCmd->currentData().toString());
            if (QComboCmd->currentIndex() != 0) Message.append(QComboCmd->currentData().toString());
            if (!LineEditInstruction->text().isEmpty()) Message.append(LineEditInstruction->text().toUpper());
            Message.append(">");
            LineEditInstruction->clear();
            InstructionManuel->setText(Message);
        }
        port->write(InstructionManuel->text().toLatin1());
        TerminalUSBSerial->moveCursor(QTextCursor::End);
        TerminalUSBSerial->insertPlainText(InstructionManuel->text().toLatin1());
        TerminalUSBSerial->moveCursor(QTextCursor::End);
        TerminalUSBSerial->insertPlainText("\n");
        TerminalUSBSerial->moveCursor(QTextCursor::End);
        TerminalUSBSerial->insertPlainText(Interpretation(InstructionManuel->text()));
        InstructionManuel->clear();
        QComboCmd->setCurrentIndex(0);
    }
    //2ieme cas on envois une commande venant d'une autre fonction
    else port->write(commandeToSend.data());
    port->flush();
    InstructionManuel->clear();
#ifdef DEBUG_COMANDSAVE
    std::cout << "/" << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
}

//Fonction qui lit tout ce qui se présente sur le port série, lorsque la connection est ouverte
//Quand un signal de synchronisation se présente, il est traité et on affiche un texte adapté
void ComArduino::onReadyRead()
{
#ifdef DEBUG_COMANDSAVE
    std::cout << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
    if (port->bytesAvailable()) {
        TerminalUSBSerial->moveCursor(QTextCursor::End);
        QString texteRecu = QString::fromLatin1(port->readAll());
        TerminalUSBSerial->insertPlainText(texteRecu);
        TerminalUSBSerial->moveCursor(QTextCursor::End);
        //Insérer ici le traitement des commandes reçue du système
        //On affiche la commande en brute, puis elle est interprétée
        //et on affiche une phare traduisant le message
        for (int i = 0; i < texteRecu.length(); i++) {
            if (CommandeEnCours)
                CommandeRecu.append(texteRecu.at(i));
            if (CommandeEnCours && texteRecu.at(i) == '>') {
                CommandeEnCours = false;
                TerminalUSBSerial->insertPlainText(Interpretation(CommandeRecu));
            }
            if (!CommandeEnCours && texteRecu.at(i) == '<') {
                CommandeEnCours = true;
                CommandeRecu = "<";
            }
        }
    }
#ifdef DEBUG_COMANDSAVE
    std::cout << "/" << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
}

//Fonction qui met a jour la liste des ports disponibles
void ComArduino::onPortAddedOrRemoved()
{
#ifdef DEBUG_COMANDSAVE
    std::cout << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
    QString current = PortBox->currentText();
    //Création d'une lite vide que l'on va remplir avec tout les modules arduino connecté
    QStringList PortArduino;
    PortBox->blockSignals(true);
    PortBox->clear();
    //On fait le tri, pour ne garder que les ports avec modules Arduino
    foreach (QextPortInfo info, QextSerialEnumerator::getPorts()) {
        QString portl = info.portName;
        if (portl.contains("ttyACM"))
        {
            if (!PortArduino.contains(info.physName))
            {
                //Si c'est bien un Arduino "ttyACM.", on l'ajoute a la liste des modules Arduino connecté
                 PortArduino.append(info.physName);
            }
        }
    }
    PortBox->addItems(PortArduino);
    if (PortBox->findText(current) != -1)
    {
        PortBox->setCurrentIndex(PortBox->findText(current));
    }
    if (PortBox->currentText() != current && !port->isOpen())
    {
        TerminalUSBSerial->clear();
        TerminalUSBSerial->insertPlainText(Const_QString(0));

    }
    PortBox->blockSignals(false);
#ifdef DEBUG_COMANDSAVE
    std::cout << "/" << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
}

//Fonction Globale pour les const QString
QString ComArduino::Const_QString(int type)
{
#ifdef DEBUG_COMANDSAVE
    std::cout << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
    //Variable globale pour les const QString
    static const char* TitreEtConstante[] = {
       QT_TR_NOOP("Lecture de l'Arduino...")
    };
#ifdef DEBUG_COMANDSAVE
    std::cout << "/" << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
    return tr(TitreEtConstante[type]);
}

//Fonction pour appliquer et réappliquer tous les labels en fonction des demande de traduction
void ComArduino::retranslate()
{
#ifdef DEBUG_COMANDSAVE
    std::cout << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
    QueryModeBox->setItemText(0,tr("Polling","Méthode utilisée"));
    QueryModeBox->setItemText(1,tr("EventDriven","Méthode Utilisée"));
    ButtonEffLumineux->setText(tr("Envoyer\nEffets Lumineux"));
    ButtonConditionsArduino->setText(tr("Envoyer\nConditions horaire"));
    ButtonHeurPCArduino->setText(tr("Heure PC -> Arduino"));
    ButtonRAZRAMArduino->setText(tr("RAZ RAM Arduino"));
    ButtonSend->setText(tr("Envoyer"));
    LabelBaudeRate->setText(tr("Baude Rate"));
    LabelParity->setText(tr("Parity"));
    LabelDataBit->setText(tr("Data Bit"));
    LabelStopBit->setText(tr("Stop Bit"));
    LabelQueryMode->setText(tr("Query Mode"));
    LabelPort->setText(tr("Liste des ports"));
    LabelTimeOut->setText(tr("Time Out"));
    TabDock->setTabText(0, tr("Envoi/Reception"));
    TabDock->setTabToolTip(0, tr("Envois et réception de donnée avec le module Arduino"));
    TabDock->setTabText(1, tr("Paramètres"));
    TabDock->setTabToolTip(1, tr("Outil pour s'assurer que le PC communique bien avec l'Arduino"));
#ifdef DEBUG_COMANDSAVE
    std::cout << "/" << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
}

//Slot sur un changement de module pour envoyer un message système vers un module
void ComArduino::onModuleChanged(int index)
{
#ifdef DEBUG_COMANDSAVE
    std::cout << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
    if (index == 0) {
        QComboTypeCmd->setHidden(true);
        QComboCmd->setHidden(true);
        LineEditInstruction->setHidden(true);
        LabelTypeCmd->setHidden(true);
        LabelCmd->setHidden(true);
        LabelInstruction->setHidden(true);
        LabelAideParametre->setHidden(true);
        Convert->setHidden(true);
    }
    else {
        QComboTypeCmd->setHidden(true);
        QComboCmd->setHidden(true);
        LineEditInstruction->setHidden(true);
        LabelTypeCmd->setHidden(true);
        LabelCmd->setHidden(true);
        LabelInstruction->setHidden(true);
        LabelAideParametre->setHidden(true);
        Convert->setHidden(true);
        QComboTypeCmd->clear();
        QComboTypeCmd->addItem("");
        //On peuple la comboboxTypeCmd
        QDomElement element = docCommandeModule->documentElement();
        for(QDomElement qde = element.firstChildElement(); !qde.isNull(); qde = qde.nextSiblingElement())
        {
            //Prend le nom et vérifie l'initialisation du mode
            if (qde.attribute("Nom") == QComboModul->currentText())
            {
                for(QDomElement qdea = qde.firstChildElement(); !qdea.isNull(); qdea = qdea.nextSiblingElement())
                {
                    if (qdea.tagName() == "TypeCmd")
                    {
                        QComboTypeCmd->setHidden(false);
                        LabelTypeCmd->setHidden(false);
                        QComboTypeCmd->addItem(qdea.attribute("Nom"), qdea.attribute("Cmd"));
                    }
                }
            }
        }
    }
#ifdef DEBUG_COMANDSAVE
    std::cout << "/" << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
}

//Slot sur un changement de type de commande pour envoyer un message système vers un module
void ComArduino::onTypeCmdChanged(int index)
{
#ifdef DEBUG_COMANDSAVE
    std::cout << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
    if (index == 0) {
        QComboCmd->setHidden(true);
        LineEditInstruction->setHidden(true);
        LabelCmd->setHidden(true);
        LabelInstruction->setHidden(true);
        LabelAideParametre->setHidden(true);
        Convert->setHidden(true);
    }
    else {
        QComboCmd->setHidden(true);
        LineEditInstruction->setHidden(true);
        LabelCmd->setHidden(true);
        LabelInstruction->setHidden(true);
        LabelAideParametre->setHidden(true);
        Convert->setHidden(true);
        QComboCmd->clear();
        QComboCmd->addItem("");
        //On peuple la comboboxTypeCmd
        QDomElement element = docCommandeModule->documentElement();
        for(QDomElement qde = element.firstChildElement(); !qde.isNull(); qde = qde.nextSiblingElement())
        {
            //Prend le nom et vérifie l'initialisation du mode
            if (qde.attribute("Nom") == QComboModul->currentText())
            {
                for(QDomElement qdea = qde.firstChildElement(); !qdea.isNull(); qdea = qdea.nextSiblingElement())
                {
                    if (qdea.attribute("Nom") == QComboTypeCmd->currentText())
                    {
                        for(QDomElement qdeab = qdea.firstChildElement(); !qdeab.isNull(); qdeab = qdeab.nextSiblingElement())
                        {
                            if (qdeab.tagName() == "Commande") {
                                QComboCmd->setHidden(false);
                                LabelCmd->setHidden(false);
                                QComboCmd->addItem(qdeab.attribute("Nom"), qdeab.attribute("Cmd"));
                            }
                        }
                    }
                }
            }
        }
    }
#ifdef DEBUG_COMANDSAVE
    std::cout << "/" << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
}

//Slot sur un changement de commande pour envoyer un message système vers un module
void ComArduino::onCmdChanged(int index)
{
#ifdef DEBUG_COMANDSAVE
    std::cout << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
    if (index == 0) {
        LineEditInstruction->setHidden(true);
        LineEditInstruction->setInputMask("");
        LabelInstruction->setHidden(true);
        LabelAideParametre->setHidden(true);
        Convert->setHidden(true);
    }
    else {
        LineEditInstruction->setHidden(true);
        LineEditInstruction->setInputMask("");
        LabelInstruction->setHidden(true);
        LabelAideParametre->setHidden(true);
        Convert->setHidden(true);
        //On peuple la comboboxTypeCmd
        QDomElement element = docCommandeModule->documentElement();
        for(QDomElement qde = element.firstChildElement(); !qde.isNull(); qde = qde.nextSiblingElement())
        {
            //Prend le nom et vérifie l'initialisation du mode
            if (qde.attribute("Nom") == QComboModul->currentText())
            {
                for(QDomElement qdea = qde.firstChildElement(); !qdea.isNull(); qdea = qdea.nextSiblingElement())
                {
                    if (qdea.attribute("Nom") == QComboTypeCmd->currentText())
                    {
                        for(QDomElement qdeab = qdea.firstChildElement(); !qdeab.isNull(); qdeab = qdeab.nextSiblingElement())
                        {
                            if (qdeab.attribute("Nom") == QComboCmd->currentText())
                            {
                                for(QDomElement qdeabc = qdeab.firstChildElement(); !qdeabc.isNull(); qdeabc = qdeabc.nextSiblingElement())
                                {
                                    if (qdeabc.tagName() == "Parametre") {
                                        LineEditInstruction->setHidden(false);
                                        LineEditInstruction->clear();
                                        if (qdeabc.hasAttribute("Format")) {
                                           QString Format = qdeabc.attribute("Format");
                                           LineEditInstruction->setInputMask(Format);
                                        }
                                        LabelInstruction->setHidden(false);
                                        LabelAideParametre->setHidden(false);
                                        LabelAideParametre->setText(qdeabc.attribute("Aide"));
                                        if (qdeabc.attribute("Hexa") == "true") {
                                            Convert->setHidden(false);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
#ifdef DEBUG_COMANDSAVE
    std::cout << "/" << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
}

//Reception d'une valeur hexadécimal
void ComArduino::onHexaCame(QString Val)
{
#ifdef DEBUG_COMANDSAVE
    std::cout << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
    QString Laligne = LineEditInstruction->text();
    QString Format = LineEditInstruction->inputMask();
    if (Format != "") {
       //Application du format à LaLigne avec la nouvelle Val Valeur
        if (Val.length() == 1 || Val.length() == 3)
            Val.prepend("0");
        int index = Laligne.indexOf(' ');
        Laligne.prepend(Val);
    }
    else {
        if (Val.length() == 1 || Val.length() == 3)
            Laligne.append("0");
        Laligne.append(Val);
    }
    LineEditInstruction->setText(Laligne.toUpper());
#ifdef DEBUG_COMANDSAVE
    std::cout << "/" << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
}

//Fonction qui traduit un message reçu pour le rendre plus lisible à l'utilisateur
QString ComArduino::Interpretation(QString Message)
{
#ifdef DEBUG_COMANDSAVE
    std::cout << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
    QString Destination = "Pour ", ValeurAcomparer = "", Provenance = "", TextComplet = "";
    //On recherche le destinataire du message
    ValeurAcomparer = Message.mid(1,3);
    QDomElement element = docCommandeModule->documentElement();
    for(QDomElement qde = element.firstChildElement(); !qde.isNull(); qde = qde.nextSiblingElement())
    {
        //On compare les commandes
        if (qde.attribute("Cmd") == ValeurAcomparer)
        {
            Destination.append(qde.attribute("Nom"));
            for(QDomElement qdea = qde.firstChildElement(); !qdea.isNull(); qdea = qdea.nextSiblingElement())
            {
                //On compare les type de commandes
                if (qdea.attribute("Cmd") == Message.mid(7,1))
                {
                    TextComplet.append(qdea.attribute("Nom"));
                    TextComplet.append(", ");
                    for(QDomElement qdeab = qdea.firstChildElement(); !qdeab.isNull(); qdeab = qdeab.nextSiblingElement())
                    {
                        //On compare les type de commandes
                        bool test = false;
                        //Si Cmd correspond on procéde à l'affichage des données
                        if (qdeab.attribute("Cmd") == Message.mid(8,1)) test = true;
                        //Si il y a un attribu de type Provenance, on le vérifie et éventuellement annule l'affichage
                        //C'est pour les cas où la commande est identique pour des choses différentes sur des modules différent
                        if (qdeab.hasAttribute("Prov"))
                            if (qdeab.attribute("Prov") != Message.mid(4,3))
                                if (test) test = false;
                        if (test)
                        {
                            TextComplet.append(qdeab.attribute("Nom"));
                            TextComplet.append(": ");
                            //Ajouter si possible le convertisseur Hexa vers décimal
                            if (qdeab.attribute("Hexa") == "true")
                                TextComplet.append(ConvertHexaDec(qdeab.attribute("Format"), Message.mid(9)));
                            //En XML l'attribu "Hexa" si la chaine contient du binaire ou de l'Hexa
                            else {
                                TextComplet.append(Message.mid(9));
                                TextComplet.chop(1);
                            }
                            TextComplet.append(" ( ");
                            TextComplet.append(qdeab.firstChildElement().attribute("Aide"));
                            TextComplet.append(" )");
                        }
                    }
                }
            }
        }
    }
    //On recherche l'envoyeur
    ValeurAcomparer = Message.mid(4,3);
    Provenance.append(", provenant de ");
    if (ValeurAcomparer == "M00")
        Provenance.append("Programme PC");
    else {
        QDomElement element = docCommandeModule->documentElement();
        for(QDomElement qde = element.firstChildElement(); !qde.isNull(); qde = qde.nextSiblingElement())
        {
            //On compare les commandes
            if (qde.attribute("Cmd") == ValeurAcomparer)
            {
                Provenance.append(qde.attribute("Nom"));
            }
        }
    }
    Provenance.append(", "),
    Destination.append(Provenance);
    Destination.append(TextComplet);
    Destination.append("\n");
    TextComplet = Destination;
    return TextComplet;
#ifdef DEBUG_COMANDSAVE
    std::cout << "/" << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
}

//Converti les valeurs hexa contenue dans Value en décimal
QString ComArduino::ConvertHexaDec(QString Format, QString Value)
{
#ifdef DEBUG_COMANDSAVE
    std::cout << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
    //On transforme la string format pour indiquer uniquement les booléen et les hexa
    //FormatCorr => identifie les valeurs a remplacée, FormatCopy => Caractère de séparation
    QString FormatCorr = "", FormatCopy = "", result = "";
    //length longeur de la chaine, offset => lorsque l'on rencontre un \, il y a un décalage de 1 char dans le format
    int length = Format.length(), offset = 0;
    //La lettre 'C' est inscrite dans les cases vide pour les identifiers proprement
    for (int i = 0; i < length; i++) {
        switch (Format.at(i + offset).toLatin1()) {
        case 'H':
            FormatCorr.append(Format.at(i + offset));
            FormatCopy.append('C');
            break;
        case 'B':
            FormatCorr.append(Format.at(i + offset));
            FormatCopy.append('C');
            break;
        case 'A':
            FormatCorr.append(Format.at(i + offset));
            FormatCopy.append('C');
            break;
        case 'N':
            FormatCorr.append(Format.at(i + offset));
            FormatCopy.append('C');
            break;
        case '\\':
            //Ce carractère annule le carractère suivant
            offset += 1;
            length -= 1;
            FormatCorr.append('C');
            FormatCopy.append(Format.at(i + offset));
            break;
        default:
            FormatCorr.append('C');
            if (Value.at(i) == Format.at(i)) {
               FormatCopy.append(Format.at(i + offset));
            }
            else {
                FormatCopy.append('D');
            }
            break;
        }
    }
    //result.append("<");
    //result.append(Format);
    //result.append("><");
    //result.append(FormatCorr);
    //result.append("><");
    //result.append(FormatCopy);
    //result.append(">");
    //Traitement de la chaine
    for (int i = 0; i < length; i++) {
        if (FormatCorr.at(i).toLatin1() == 'H') {
            result.append(" ");
            if (i + 4 <= length) {
                if (FormatCorr.at(i + 2).toLatin1() == 'H') {
                    bool ok;
                    result.append(QString::number(Value.mid(i,4).toInt(&ok, 16)));
                    //result.append(Value.mid(i,4));
                    i += 3;
                }
                else {
                    bool ok;
                    result.append(QString::number(Value.mid(i,2).toInt(&ok, 16)));
                    //result.append(Value.mid(i,2));
                    i += 1;
                }
            }
            else {
                bool ok;
                result.append(QString::number(Value.mid(i,2).toInt(&ok, 16)));
                //result.append(Value.mid(i,2));
                i += 1;
            }
            result.append(" ");
        }
        else if (FormatCorr.at(i).toLatin1() == 'B') {
            if (Value.at(i).toLatin1() == '1')
                result.append(" On ");
            else
                result.append(" Off ");
        }
        else if (FormatCorr.at(i).toLatin1() == 'A' || FormatCorr.at(i).toLatin1() == 'N') {
            result.append(" ");
            result.append(Value.at(i).toLatin1());
            while (FormatCorr.length() > i + 1) {
               if (FormatCorr.at(i).toLatin1() == FormatCorr.at(i + 1).toLatin1()) {
                   i++;
                   result.append(Value.at(i).toLatin1());
               }
               else {
                   result.append(" ");
                   break;
               }
            }
        }
        else {
            result.append(Value.at(i).toLatin1());
        }
    }
#ifdef DEBUG_COMANDSAVE
    std::cout << "/" << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
    return result;
}
