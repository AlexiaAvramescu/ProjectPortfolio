#pragma once

#include <QWidget>
#include "ui_LoginPage.h"
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class LoginPageClass; };
QT_END_NAMESPACE

enum class WindowState {
	LOGIN,
	REGISTER
};

class LoginPage : public QWidget
{
	Q_OBJECT

public:
	LoginPage(QWidget* parent = nullptr);
	~LoginPage();

	Ui::LoginPageClass* GetUi();
	void ChangeToLoginPage();

protected:     
	void paintEvent(QPaintEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;

private:
	void CleanTextFields();
	void OnLogin();
	void OnRegister();
	bool ValidateInput();

private slots:
	void ChangeToLogin();
	void ChangeToRegister();
	void Proceed();

signals:
	void loginSuccessful();
	void SendLoginToServer();
	void SendRegisterToServer();

private:
	Ui::LoginPageClass* ui;
	WindowState m_windowState;
};
