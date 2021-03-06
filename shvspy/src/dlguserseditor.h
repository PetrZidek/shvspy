#ifndef DLGUSERSEDITOR_H
#define DLGUSERSEDITOR_H

#include <QDialog>

#include <shv/iotqt/rpc/rpcresponsecallback.h>
#include <shv/chainpack/rpcvalue.h>

namespace Ui {
class DlgUsersEditor;
}

class DlgUsersEditor : public QDialog
{
	Q_OBJECT

public:
	explicit DlgUsersEditor(QWidget *parent, shv::iotqt::rpc::ClientConnection *rpc_connection);
	~DlgUsersEditor();
	void init(const std::string &path);

private:
	Ui::DlgUsersEditor *ui;

	void listUsers();
	QString selectedUser();

	void onAddUserClicked();
	void onDelUserClicked();
	void onEditUserClicked();
	void onTableUsersDoubleClicked(QModelIndex ix);

    std::string aclEtcUsersNodePath();

	shv::iotqt::rpc::ClientConnection *m_rpcConnection = nullptr;
	std::string m_aclEtcNodePath;
};

#endif // DLGUSERSEDITOR_H
