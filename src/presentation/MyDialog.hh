//
// Created by MichaelBrunner on 04/09/2025.
//

#pragma once

#include <QDockWidget>
#include <QString>

class QLabel;
class QLineEdit;
class QDialogButtonBox;


#if defined(_WIN32)
#if defined(PRESENTATION_EXPORTS)
#define PRESENTATION_API __declspec(dllexport)
#else
#define PRESENTATION_API __declspec(dllimport)
#endif
#else
#define PRESENTATION_API
#endif


class PRESENTATION_API MyDialog : public QDockWidget {
    Q_OBJECT
public:
    explicit MyDialog(QWidget* parent = nullptr);

    // Convenience accessor
    QString text() const;

signals:
    void accepted();

private:
    QLabel* m_label;
    QLineEdit* m_lineEdit;
    QDialogButtonBox* m_buttonBox;
};