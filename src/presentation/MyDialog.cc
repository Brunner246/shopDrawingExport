//
// Created by MichaelBrunner on 04/09/2025.
//

#include "MyDialog.hh"


#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>

MyDialog::MyDialog(QWidget* parent)
    : QDockWidget(parent),
      m_label(new QLabel(tr("Enter text:"), this)),
      m_lineEdit(new QLineEdit(this)),
      m_buttonBox(new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this))
{
    setWindowTitle(tr("My Panel"));

    auto* central = new QWidget(this);

    auto* formLayout = new QHBoxLayout();
    formLayout->addWidget(m_label);
    formLayout->addWidget(m_lineEdit);

    auto* mainLayout = new QVBoxLayout(central);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(m_buttonBox);

    connect(m_lineEdit, &QLineEdit::textChanged, this, [this](const QString& s) {
        m_buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!s.trimmed().isEmpty());
    });
    m_buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!m_lineEdit->text().trimmed().isEmpty());

    connect(m_buttonBox, &QDialogButtonBox::accepted, this, [this]() {
        emit accepted();
        this->close();
    });
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QWidget::close);

    setWidget(central);
}

QString MyDialog::text() const {
    return m_lineEdit->text();
}