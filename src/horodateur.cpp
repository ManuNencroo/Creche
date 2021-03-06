#include "horodateur.h"

Horodateur::Horodateur(const QString & title, QWidget *parent , Qt::WindowFlags flags) :
    QDockWidget(title, parent, flags)
{
#ifdef DEBUG_COMANDSAVE
    std::cout << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
    //Aplication d'un style au dock
    setStyleSheet("QDockWidget::title {text-align: centre; background: rgb(0,189,138); color: rgb(0,0,138); border-style: inset; border-width: 2px; border-radius: 2px; border-color: rgb(0,0,138); font: 8pt; padding: 0px; height: 20px} QDockWidget::float-button { border: 1px solid; background: rgb(0,189,138); padding: 0px} QDockWidget::float-button:hover { background: rgb(0,0,138) } QDockWidget {border-style: outset; border-width: 2px; border-radius: 4px; border-color: rgb(0,0,138)}");
    setFeatures(QDockWidget::NoDockWidgetFeatures);
    setFeatures(QDockWidget::DockWidgetMovable);
    setFeatures(QDockWidget::DockWidgetFloatable);

    QFont font("Verdana", 9);
    //MainLayout pour le scrollArea
    MainLayout = new QVBoxLayout;
    MainLayout->setContentsMargins(1,1,1,1);
    MainLayout->setSpacing(1);
    //Layout de base
    LayoutBase = new QVBoxLayout;
    LayoutBase->setContentsMargins(0,0,0,0);
    LayoutBase->setSpacing(1);
    //Layout start date
    LayoutStartDate = new QHBoxLayout;
    LayoutStartDate->setContentsMargins(0,0,0,0);
    LayoutStartDate->setSpacing(2);
    //Layout End date
    LayoutEndDate = new QHBoxLayout;
    LayoutEndDate->setContentsMargins(0,0,0,0);
    LayoutEndDate->setSpacing(2);
    //Layout pour la zone de date
    LayoutDateEdite = new QVBoxLayout;
    LayoutDateEdite->setContentsMargins(1,1,1,1);
    LayoutDateEdite->setSpacing(1);
    LayoutDateEdite->setAlignment(Qt::AlignJustify);
    //Layout pour la zone de sélection d'un jour
    LayoutJourSelect = new QVBoxLayout;
    LayoutJourSelect->setContentsMargins(1,1,1,1);
    LayoutJourSelect->setSpacing(1);
    LayoutJourSelect->setAlignment(Qt::AlignJustify);
    //Layout pour la zone de sélection d'heure
    LayoutTimeEdit = new QVBoxLayout;
    LayoutTimeEdit->setContentsMargins(1,1,1,1);
    LayoutTimeEdit->setSpacing(1);
    LayoutTimeEdit->setAlignment(Qt::AlignJustify);
    //Label date début
    LabelStartDate = new QLabel("");
    LabelStartDate->setFont(font);
    LabelStartDate->setFixedHeight(20);
    //Label date de fin
    LabelEndDate = new QLabel("");
    LabelEndDate->setFont(font);
    LabelEndDate->setFixedHeight(20);
    //Label selectionner un jour de la semaine
    LabelJourSelect = new QLabel("");
    LabelJourSelect->setFont(font);
    LabelJourSelect->setFixedHeight(20);
    //Label sélectionner une heure de début
    LabelTimeStart = new QLabel("");
    LabelTimeStart->setFont(font);
    LabelTimeStart->setFixedHeight(20);
    //Label sélectionner une heure de fin
    LabelTimeEnd = new QLabel("");
    LabelTimeEnd->setFont(font);
    LabelTimeEnd->setFixedHeight(20);
    //Boutton pour ouvrir la fenètre de sélection de date start
    ButtonStartDate = new QPushButton("...");
    ButtonStartDate->setFont(font);
    ButtonStartDate->setFixedHeight(20);
    ButtonStartDate->setFixedWidth(22);
    //Boutton pour ouvrir la fenètre de sélection de date end
    ButtonEndDate = new QPushButton("...");
    ButtonEndDate->setFont(font);
    ButtonEndDate->setFixedHeight(20);
    ButtonEndDate->setFixedWidth(22);
    //Boutton pour valider les données introduite
    ButtonValider = new QPushButton;
    ButtonValider->setFont(font);
    ButtonValider->setFixedHeight(20);
    ButtonValider->setVisible(false);
    //Boutton pour supprimer ce qui est programmé
    ButtonDelete = new QPushButton;
    ButtonDelete->setFixedHeight(20);
    ButtonDelete->setFont(font);
    //Widget de base qui accueil le main layout
    Base = new QWidget;
    //Widget de taille appliquer au scrollArea
    WidgetScrollArea = new QWidget;
    WidgetScrollArea->setFixedHeight(213);
    //Widget pour les différentes zones,
    //Chaque widget est rendu visible ou invisible en fonction de la sélection
    //dans la comboBox
    DateEdite = new QWidget;
    DateEdite->setMinimumHeight(80);
    //Widget pour la sélection d'un jour
    SelectJour = new QWidget;
    SelectJour->setMaximumHeight(40);
    //Widget pour la selection d'heures
    TimeEdit = new QWidget;
    TimeEdit->setMinimumHeight(80);
    //ComboBox pour sélectionner le type de condition horaire
    ComboBoxSelect = new QComboBox;
    ComboBoxSelect->addItem(tr("Faire sélection"), Vide);
    ComboBoxSelect->addItem(tr("Période, de date à date"), Periode);
    ComboBoxSelect->addItem(tr("Tous les jours"), Journalier);
    ComboBoxSelect->addItem(tr("Un jour de la semaine"), Hebdomadaire);
    ComboBoxSelect->addItem(tr("View"), View);
    ComboBoxSelect->setFont(font);
    //ComboBox jour de la semaine sélectionné
    JourSelect = new QComboBox;
    JourSelect->addItem(tr("Lundi"), 1);
    JourSelect->addItem(tr("Mardi"), 2);
    JourSelect->addItem(tr("Mercredi"), 3);
    JourSelect->addItem(tr("Jeudi"), 4);
    JourSelect->addItem(tr("Vendredi"), 5);
    JourSelect->addItem(tr("Samedi"), 6);
    JourSelect->addItem(tr("Dimanche"), 7);
    JourSelect->setFont(font);
    JourSelect->setFixedHeight(20);
    //Date edit pour la date de début
    DateEditeStart = new QDateEdit;
    DateEditeStart->setFont(font);
    DateEditeStart->setFixedHeight(20);
    //Date edit pour la date de fin
    DateEditeEnd = new QDateEdit;
    DateEditeEnd->setFont(font);
    DateEditeEnd->setFixedHeight(20);
    //Scroll area pour acuellir les layouts avec les champs de travail
    ScrollArea = new QScrollArea;
    ScrollArea->setContentsMargins(0,0,0,0);
    ScrollArea->setWidgetResizable(true);
    ScrollArea->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    ScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //Time edit pour l'heure de début
    TimeEditStart = new QTimeEdit;
    TimeEditStart->setFont(font);
    TimeEditStart->setFixedHeight(20);
    //Time edite pour l'heure de fin
    TimeEditEnd = new QTimeEdit;
    TimeEditEnd->setFont(font);
    TimeEditEnd->setFixedHeight(20);
    //View de text edit qui permet de visuliser ce qui est programmé
    TextView = new QPlainTextEdit;
    TextView->setReadOnly(true);
    TextView->setFont(font);

    //Ajout des widgets et layout pour l'ensemble Date
    LayoutStartDate->addWidget(DateEditeStart);
    LayoutStartDate->addWidget(ButtonStartDate);
    LayoutEndDate->addWidget(DateEditeEnd);
    LayoutEndDate->addWidget(ButtonEndDate);
    LayoutDateEdite->addWidget(LabelStartDate);
    LayoutDateEdite->addLayout(LayoutStartDate);
    LayoutDateEdite->addWidget(LabelEndDate);
    LayoutDateEdite->addLayout(LayoutEndDate);
    DateEdite->setLayout(LayoutDateEdite);
    DateEdite->setVisible(false);
    //Ajout de widget pour l'ensemble jour
    LayoutJourSelect->addWidget(LabelJourSelect);
    LayoutJourSelect->addWidget(JourSelect);
    SelectJour->setLayout(LayoutJourSelect);
    SelectJour->setVisible(false);
    //Ajout des widget pour l'ensemble heure
    LayoutTimeEdit->addWidget(LabelTimeStart);
    LayoutTimeEdit->addWidget(TimeEditStart);
    LayoutTimeEdit->addWidget(LabelTimeEnd);
    LayoutTimeEdit->addWidget(TimeEditEnd);
    TimeEdit->setLayout(LayoutTimeEdit);
    TimeEdit->setVisible(false);
    //Ajout des widgets sur le main layout
    MainLayout->addWidget(DateEdite);
    MainLayout->addWidget(SelectJour);
    MainLayout->addWidget(TimeEdit);
    MainLayout->addWidget(ButtonValider);
    MainLayout->addWidget(TextView);
    MainLayout->addWidget(ButtonDelete);
    //Application du layout principale sur un widget dont ont impose la taille
    WidgetScrollArea->setLayout(MainLayout);
    //Application au layout de base
    ScrollArea->setWidget(WidgetScrollArea);
    LayoutBase->addWidget(ComboBoxSelect);
    LayoutBase->addWidget(ScrollArea);
    //Application au dock
    Base->setLayout(LayoutBase);
    setWidget(Base);

    //On met à zero les conditions horaires
    ResetAffichage();
    //On place tous les labels
    retranslate();

    connect(ComboBoxSelect, SIGNAL(currentIndexChanged(int)), SLOT(ChangeType(int)));
    connect(ButtonValider, SIGNAL(clicked()), SLOT(PrepareToSend()));
    connect(ButtonDelete, SIGNAL(clicked()), SLOT(BouttonDeleteClick()));
#ifdef DEBUG_COMANDSAVE
    std::cout << "/" << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
}

Horodateur::~Horodateur()
{
#ifdef DEBUG_COMANDSAVE
    std::cout << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
    delete TimeEditStart;
    delete TimeEditEnd;
    delete DateEditeStart;
    delete DateEditeEnd;
    delete ComboBoxSelect;
    delete JourSelect;
    delete ButtonEndDate;
    delete ButtonStartDate;
    delete ButtonValider;
    delete LabelEndDate;
    delete LabelJourSelect;
    delete LabelStartDate;
    delete LabelTimeEnd;
    delete LabelTimeStart;
    delete LayoutStartDate;
    delete LayoutEndDate;
    delete LayoutDateEdite;
    delete LayoutJourSelect;
    delete LayoutTimeEdit;
    delete DateEdite;
    delete SelectJour;
    delete TimeEdit;
    delete MainLayout;
    delete WidgetScrollArea;
    delete ScrollArea;
    delete LayoutBase;
    delete Base;
#ifdef DEBUG_COMANDSAVE
    std::cout << "/" << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
}

//Fonction qui adapte la zone visible du dock
void Horodateur::ChangeType(int index)
{
#ifdef DEBUG_COMANDSAVE
    std::cout << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
    //On récupère la valeur de la comboBox et on caste suivant Type (enum)
    Type Select = static_cast<Type>(ComboBoxSelect->itemData(index).toInt());

    switch(Select)
    {
    case Periode:
        WidgetScrollArea->setFixedHeight(107);
        DateEdite->setVisible(true);
        SelectJour->setVisible(false);
        TimeEdit->setVisible(false);
        ButtonValider->setVisible(true);
        TextView->setVisible(false);
        ButtonDelete->setVisible(false);
        break;
    case Vide:
        WidgetScrollArea->setFixedHeight(20);
        DateEdite->setVisible(false);
        SelectJour->setVisible(false);
        TimeEdit->setVisible(false);
        ButtonValider->setVisible(false);
        TextView->setVisible(false);
        ButtonDelete->setVisible(false);
        break;
    case Hebdomadaire:
        WidgetScrollArea->setFixedHeight(150);
        DateEdite->setVisible(false);
        SelectJour->setVisible(true);
        TimeEdit->setVisible(true);
        ButtonValider->setVisible(true);
        TextView->setVisible(false);
        ButtonDelete->setVisible(false);
        break;
    case Journalier:
        WidgetScrollArea->setFixedHeight(107);
        DateEdite->setVisible(false);
        SelectJour->setVisible(false);
        TimeEdit->setVisible(true);
        ButtonValider->setVisible(true);
        TextView->setVisible(false);
        ButtonDelete->setVisible(false);
        break;
    case View:
        if (Compilation::NbInstance() > 0)
        {
            WidgetScrollArea->setFixedHeight(500);
            DateEdite->setVisible(false);
            SelectJour->setVisible(false);
            TimeEdit->setVisible(false);
            ButtonValider->setVisible(false);
            TextView->setVisible(true);
            ButtonDelete->setVisible(true);
            TextView->clear();
            TextView->moveCursor(QTextCursor::End);
            TextView->insertPlainText(SaveXmlFile::ReturneCondHoraire());
        }
        else
        {
            WidgetScrollArea->setFixedHeight(20);
            DateEdite->setVisible(false);
            SelectJour->setVisible(false);
            TimeEdit->setVisible(false);
            ButtonValider->setVisible(false);
            TextView->setVisible(false);
            ButtonDelete->setVisible(false);
            ComboBoxSelect->setCurrentIndex(Vide);
        }
        break;
    }
#ifdef DEBUG_COMANDSAVE
    std::cout << "/" << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
}

//Slot qui prepare les conditions horaire pour la compilation
void Horodateur::PrepareToSend()
{
#ifdef DEBUG_COMANDSAVE
    std::cout << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
    //On récupère la valeur de la comboBox et on caste suivant Type (enum)
    Type Select = static_cast<Type>(ComboBoxSelect->itemData(ComboBoxSelect->currentIndex()).toInt());
    //Initialisation des variables
    CondHoraire *CondH = new CondHoraire{0,0,0,0,0,0,0,0,0,0};
    switch(Select)
    {
    case Periode:
        CondH->DMois = DateEditeStart->date().month();
        CondH->DJour = DateEditeStart->date().day();
        CondH->EMois = DateEditeEnd->date().month();
        CondH->EJour = DateEditeEnd->date().day();
        break;
    case Vide:
        QMessageBox::information(this, tr("Pas de condition Horaire"), tr("Il n'y a condition horaire programmée"), QMessageBox::Ok);
        return;
    case Hebdomadaire:
        CondH->DHeure = TimeEditStart->time().hour();
        CondH->DMinute = TimeEditStart->time().minute();
        CondH->EHeure = TimeEditEnd->time().hour();
        CondH->EMinute = TimeEditEnd->time().minute();
        CondH->DJourSem = JourSelect->currentData().toInt();
        CondH->EJourSem = JourSelect->currentData().toInt();
        break;
    case Journalier:
        CondH->DHeure = TimeEditStart->time().hour();
        CondH->DMinute = TimeEditStart->time().minute();
        CondH->EHeure = TimeEditEnd->time().hour();
        CondH->EMinute = TimeEditEnd->time().minute();
        break;
    case View:
#ifdef DEBUG_COMANDSAVE
        std::cout << "/" << func_name << "(View)" << std::endl;
#endif /* DEBUG_COMANDSAVE */
        delete CondH;
        return;
    }
#ifdef DEBUG_COMANDSAVE
    qDebug() << "Condition horaire:" << CondH->DMois << CondH->EMois << CondH->DJour << CondH->EJour << CondH->DJourSem << CondH->EJourSem << CondH->DHeure << CondH->EHeure << CondH->DMinute << CondH->EMinute;
#endif /* DEBUG_COMANDSAVE */
    emit SendCondHoraire(CondH);
    //Normalement CondH est supprimé après sa lecture, on le delete seulement s'il n'a pas été correctement supprimé
    if (!CondH)
#ifdef DEBUG_COMANDSAVE
    {
        qDebug() << "Demande de delete CondHoraire";
        delete CondH;
    }
#else /* DEBUG_COMANDSAVE */
        delete CondH;
#endif /* DEBUG_COMANDSAVE */
#ifdef DEBUG_COMANDSAVE
    std::cout << "/" << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
}

//Slot pour le boutton delete
void Horodateur::BouttonDeleteClick()
{
#ifdef DEBUG_COMANDSAVE
    std::cout << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
    emit DemandeSuppCondHoraire();
    TextView->clear();
    TextView->moveCursor(QTextCursor::End);
    TextView->insertPlainText(SaveXmlFile::ReturneCondHoraire());
#ifdef DEBUG_COMANDSAVE
    std::cout << "/" << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
}

//Fonction pour remettre a zero les conditions horaires
void Horodateur::ResetAffichage()
{
#ifdef DEBUG_COMANDSAVE
    std::cout << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
    //Le reset revient a changer de type et mettre un type "Faire sélection"
    ChangeType(0);
    ComboBoxSelect->setCurrentIndex(0);
#ifdef DEBUG_COMANDSAVE
    std::cout << "/" << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
}

//Fonction pour appliquer et réappliquer les labels suite a une demande de traduction
void Horodateur::retranslate()
{
#ifdef DEBUG_COMANDSAVE
    std::cout << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
    LabelStartDate->setText(tr("Date de début"));
    LabelEndDate->setText(tr("Date de fin"));
    LabelJourSelect->setText(tr("Choisir un jour"));
    LabelTimeStart->setText(tr("Heure de début"));
    LabelTimeEnd->setText(tr("Heure de fin"));
    ButtonValider->setText(tr("Valider"));
    ButtonDelete->setText(tr("Delete"));
    ComboBoxSelect->setItemText(0, tr("Faire sélection"));
    ComboBoxSelect->setItemText(1, tr("Période, de date à date"));
    ComboBoxSelect->setItemText(2, tr("Tous les jours"));
    ComboBoxSelect->setItemText(3, tr("Un jour de la semaine"));
    ComboBoxSelect->setItemText(4, tr("View"));
    JourSelect->setItemText(0,tr("Lundi"));
    JourSelect->setItemText(1,tr("Mardi"));
    JourSelect->setItemText(2,tr("Mercredi"));
    JourSelect->setItemText(3,tr("Jeudi"));
    JourSelect->setItemText(4,tr("Vendredi"));
    JourSelect->setItemText(5,tr("Samedi"));
    JourSelect->setItemText(6,tr("Dimanche"));
#ifdef DEBUG_COMANDSAVE
    std::cout << "/" << func_name << std::endl;
#endif /* DEBUG_COMANDSAVE */
}
