#include "LoginPage.h"
#include <QKeyEvent>
#include <QRegularExpression>


LoginPage::LoginPage(QWidget* parent)
	: QWidget{ parent }
	, ui{ new Ui::LoginPageClass() }
	, m_windowState{ WindowState::LOGIN }
{
	ui->setupUi(this);
	connect(ui->btnLogin, &QPushButton::clicked, this, &LoginPage::ChangeToLogin);
	connect(ui->btnRegister, &QPushButton::clicked, this, &LoginPage::ChangeToRegister);
	connect(ui->btnProceed, &QPushButton::clicked, this, &LoginPage::Proceed);
	ui->inputPassword->setEchoMode(QLineEdit::Password);
	ui->inputPasswordConfirm->setEchoMode(QLineEdit::Password);
}

LoginPage::~LoginPage()
{
	delete ui;
}

Ui::LoginPageClass* LoginPage::GetUi()
{
	return ui;
}

void LoginPage::ChangeToLoginPage()
{
	m_windowState = WindowState::LOGIN;
}

void LoginPage::ChangeToLogin()
{
	if (m_windowState != WindowState::LOGIN) {
		CleanTextFields();
	}
	m_windowState = WindowState::LOGIN;
	update();
}

void LoginPage::ChangeToRegister()
{
	if (m_windowState != WindowState::REGISTER) {
		CleanTextFields();
	}
	m_windowState = WindowState::REGISTER;
	update();
}

void LoginPage::paintEvent(QPaintEvent* event) {
	if (m_windowState == WindowState::LOGIN) {
		ui->title->setText("Login in order to play");
		ui->btnProceed->setText("LOGIN");
		ui->inputPasswordConfirm->setVisible(false);
		ui->labelPswConf->setVisible(false);
	}
	else {
		ui->title->setText("Register in order to play");
		ui->btnProceed->setText("REGISTER");
		ui->inputPasswordConfirm->setVisible(true);
		ui->labelPswConf->setVisible(true);
	}
}

void LoginPage::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Return) {
		Proceed();
	}
}

void LoginPage::CleanTextFields() {
	ui->inputUsername->setText("");
	ui->inputPassword->setText("");
	ui->inputPasswordConfirm->setText("");
}

void LoginPage::Proceed() {

	if (m_windowState == WindowState::LOGIN) {
		OnLogin();
	}
	else {
		OnRegister();
	}
}


void LoginPage::OnLogin() {
	if (ValidateInput())
		emit SendLoginToServer();
}


void LoginPage::OnRegister() {
	if (ValidateInput())
		emit SendRegisterToServer();
}

bool LoginPage::ValidateInput()
{
	if (ui->inputUsername->text().isEmpty() || ui->inputPassword->text().isEmpty())
	{
		QMessageBox::warning(nullptr, "Input error", "One or more of the fields are null");
		return false;
	}

	auto checkAscii = [](QString input) -> bool {
		QRegularExpression regex("^[\\x00-\\x7F]+$");
		QRegularExpressionMatch match = regex.match(input);
		return match.hasMatch() && match.capturedLength(0) == input.length();
	};

	if (!checkAscii(ui->inputPassword->text()) || !checkAscii(ui->inputUsername->text())) {
		QMessageBox::warning(nullptr, "Input error", "Invalid characters input");
		return false;
	}

	if (ui->inputPassword->text() != ui->inputPasswordConfirm->text() && m_windowState == WindowState::REGISTER) {
		QMessageBox::warning(nullptr, "Register error", "The passwords doesn't match");
		return false;
	}

	if (m_windowState == WindowState::LOGIN)
		return true;

	if (ui->inputPassword->text().size() < 6) {
		QMessageBox::warning(nullptr, "Register error", "Password needs to have more than 6 characters");
		return false;
	}

	if (QRegularExpression re("\\s"); re.match(ui->inputUsername->text()).hasMatch()
		|| re.match(ui->inputPassword->text()).hasMatch()) {
		QMessageBox::warning(nullptr, "Register error", "Password or username have spaces");
		return false;
	}
	
	return true;
}
