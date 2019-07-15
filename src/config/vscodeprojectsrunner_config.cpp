#include "vscodeprojectsrunner_config.h"
#include <KSharedConfig>
#include <KPluginFactory>
#include <krunner/abstractrunner.h>
#include <QtCore/QDir>
#include <QtWidgets/QFileDialog>

K_PLUGIN_FACTORY(VSCodeProjectsRunnerConfigFactory,
                 registerPlugin<VSCodeProjectsRunnerConfig>("kcm_krunner_vscodeprojectsrunner");)

VSCodeProjectsRunnerConfigForm::VSCodeProjectsRunnerConfigForm(QWidget *parent) : QWidget(parent) {
    setupUi(this);
}

VSCodeProjectsRunnerConfig::VSCodeProjectsRunnerConfig(QWidget *parent, const QVariantList &args) : KCModule(parent,
                                                                                                             args) {
    m_ui = new VSCodeProjectsRunnerConfigForm(this);
    auto *layout = new QGridLayout(this);
    layout->addWidget(m_ui, 0, 0);

    config = KSharedConfig::openConfig("krunnerrc")->group("Runners").group("VSCodeProjects");

    m_ui->showProjectsByApplication->setChecked(config.readEntry("appNameMatches", "true") == "true");
    m_ui->showProjectsByName->setChecked(config.readEntry("projectNameMatches", "true") == "true");
    m_ui->fileLabel->setText(
            config.readEntry("path", QDir::homePath() + "/.config/Code/User/globalStorage/alefragnani.project-manager/projects.json")
    );

    connect(m_ui->showProjectsByApplication, SIGNAL(clicked(bool)), this, SLOT(changed()));
    connect(m_ui->showProjectsByName, SIGNAL(clicked(bool)), this, SLOT(changed()));
    connect(m_ui->fileChooserButton, SIGNAL(clicked(bool)), this, SLOT(changed()));

    connect(m_ui->fileChooserButton, SIGNAL(clicked(bool)), this, SLOT(fileChooserDialog()));


    load();

}

void VSCodeProjectsRunnerConfig::save() {
    config.writeEntry("appNameMatches", m_ui->showProjectsByApplication->isChecked() ? "true" : "false");
    config.writeEntry("projectNameMatches", m_ui->showProjectsByName->isChecked() ? "true" : "false");
    config.writeEntry("path", m_ui->fileLabel->text());

    emit changed();
}

void VSCodeProjectsRunnerConfig::defaults() {
    m_ui->showProjectsByApplication->setChecked(true);
    m_ui->showProjectsByName->setChecked(true);
    m_ui->fileLabel->setText(
            QDir::homePath() + "/.config/Code/User/globalStorage/alefragnani.project-manager/projects.json"
    );
    emit changed(true);
}

void VSCodeProjectsRunnerConfig::fileChooserDialog() {
    QString jsonFile = QFileDialog::getOpenFileName(this, tr("Select file"), "", tr("Json File (*.json)"));
    if (!jsonFile.isEmpty()) {
        m_ui->fileLabel->setText(jsonFile);
    }
}


#include "vscodeprojectsrunner_config.moc"
