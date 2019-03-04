#include "subscriptionsmodel.h"

#include "../theapp.h"
#include "../servertreemodel/shvnodeitem.h"

#include <shv/chainpack/rpcvalue.h>
#include <shv/core/utils.h>
#include <shv/core/assert.h>
#include <shv/coreqt/log.h>
#include <shv/iotqt/rpc/rpc.h>

#include <QSettings>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QIcon>

namespace cp = shv::chainpack;

SubscriptionsModel::SubscriptionsModel(QObject *parent)
	: Super(parent)
{
}

SubscriptionsModel::~SubscriptionsModel()
{
}

int SubscriptionsModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent)
	int count = 0;

	for (int i = 0; i < m_shvNodeItems.count(); i++){
		QVariant v = m_shvNodeItems.at(i)->serverProperties().value("subscriptions");
		if(v.isValid()) {
			count += v.toList().size();
		}
	}
	return  count;
}

int SubscriptionsModel::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 0;

	return Columns::ColCount;
}

Qt::ItemFlags SubscriptionsModel::flags(const QModelIndex &ix) const
{
	if (!ix.isValid())
		return Qt::NoItemFlags;

	if((ix.column() == Columns::ColEnabled) ||
		(ix.column() == Columns::ColPermanent) ||
		(ix.column() == Columns::ColSubscribeAfterConnect)){
		return  Super::flags(ix) |= Qt::ItemIsEditable;
	}

	return Super::flags(ix);
}

QVariant SubscriptionsModel::data(const QModelIndex &ix, int role) const
{
/*	if(m_shvTreeNodeItem.isNull())
		return QVariant();
	const QVector<ShvMetaMethod> &mms = m_shvTreeNodeItem->methods();
	if(ix.row() < 0 || ix.row() >= mms.count())
		return QVariant();
	if(ix.column() < 0 || ix.column() >= ColCnt)
		return QVariant();

	switch (role) {
	case Qt::DisplayRole: {
		switch (ix.column()) {
		case ColMethodName:
		case ColSignature:
		case ColParams: {
			//QVariant v = m_rows.value(ix.row()).value(ix.column());
			cp::RpcValue rv = m_rows[ix.row()][ix.column()];
			return rv.isValid()? QString::fromStdString(rv.toCpon()): QVariant();
		}
		case ColResult: {
			if(m_rows[ix.row()][ColError].isIMap()) {
				cp::RpcResponse::Error err(m_rows[ix.row()][ColError].toIMap());
				return QString::fromStdString(err.message());
			}
			else {
				cp::RpcValue rv = m_rows[ix.row()][ColResult];
				if(!rv.isValid())
					return QVariant();

				static constexpr int MAX_TT_SIZE = 1024;
				std::string tts = rv.toCpon();
				if(tts.size() > MAX_TT_SIZE)
					tts = tts.substr(0, MAX_TT_SIZE) + " < ... " + std::to_string(tts.size() - MAX_TT_SIZE) + " more bytes >";

				//shvWarning() << tts << tts.length();
				return QString::fromStdString(tts);
			}
		}
		case ColFlags:
			return QString::fromStdString(m_rows[ix.row()][ix.column()].toString());
		case ColAccessGrant:
			return QString::fromStdString(m_rows[ix.row()][ix.column()].toString());
		default:
			break;
		}
		break;
	}
	case Qt::EditRole: {
		switch (ix.column()) {
		case ColResult:
		case ColParams: {
			cp::RpcValue rv = m_rows[ix.row()][ix.column()];
			return rv.isValid()? QString::fromStdString(rv.toCpon()): QVariant();
		}
		default:
			break;
		}
		break;
	}
	case RpcValueRole: {
		switch (ix.column()) {
		case ColResult:
		case ColParams: {
			cp::RpcValue rv = m_rows[ix.row()][ix.column()];
			return QVariant::fromValue(rv);
		}
		default:
			break;
		}
		break;
	}
	case Qt::DecorationRole: {
		if(ix.column() == ColBtRun) {
			bool is_notify = m_rows[ix.row()][ColFlags].toBool();
			if(!is_notify) {
				static QIcon ico_run = QIcon(QStringLiteral(":/shvspy/images/run"));
				static QIcon ico_reload = QIcon(QStringLiteral(":/shvspy/images/reload"));
				auto v = m_rows[ix.row()][ColBtRun];
				return (v.toUInt() > 0)? ico_reload: ico_run;
			}
		}
		break;
	}
	case Qt::ToolTipRole: {
		if(ix.column() == ColBtRun) {
			return tr("Call remote method");
		}
		else if(ix.column() == ColResult) {
			return data(ix, Qt::DisplayRole);
		}
		else if(ix.column() == ColFlags) {
			bool is_notify = m_rows[ix.row()][ColFlags].toUInt() & cp::MetaMethod::Flag::IsSignal;
			return is_notify? tr("Method is notify signal"): QVariant();
		}
		else {
			return data(ix, Qt::DisplayRole);
		}
	}
	default:
		break;
	}*/
	return QVariant();
}

bool SubscriptionsModel::setData(const QModelIndex &ix, const QVariant &val, int role)
{
/*	shvLogFuncFrame() << val.toString() << val.typeName() << "role:" << role;
	if(role == Qt::EditRole) {
		if(ix.column() == ColParams) {
			if(!m_shvTreeNodeItem.isNull()) {
				std::string cpon = val.toString().trimmed().toStdString();
				cp::RpcValue params;
				if(!cpon.empty()) {
					std::string err;
					params = cp::RpcValue::fromCpon(cpon, &err);
					if(!err.empty())
						shvError() << "cannot set invalid cpon data";
				}
				m_shvTreeNodeItem->setMethodParams(ix.row(), params);
				loadRow(ix.row());
				return true;
			}
		}
	}*/
	return false;
}

QVariant SubscriptionsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	QVariant ret;
	if(orientation == Qt::Horizontal) {
		if(role == Qt::DisplayRole) {
			if(section == Columns::ColServer)
				ret = tr("Server");
			else if(section == Columns::ColPath)
				ret = tr("Path");
			else if(section == Columns::ColMethod)
				ret = tr("Method");
			else if(section == Columns::ColPermanent)
				ret = tr("Permanent");
			else if(section == Columns::ColSubscribeAfterConnect)
				ret = tr("Auto subscribe");
			else if(section == Columns::ColEnabled)
				ret = tr("Enabled");
		}
		else if(role == Qt::ToolTipRole) {
			if(section == Columns::ColSubscribeAfterConnect)
				ret = tr("Subscribe after connect");
		}
	}
	return ret;
}

void SubscriptionsModel::addShvBrokerNodeItem(ShvBrokerNodeItem *nd)
{
	m_shvNodeItems.append(nd);
	connect(nd, &ShvBrokerNodeItem::subscriptionAdded, this, [this, nd](const std::string &path){
		onSubscriptionAdded(nd, path);
	});
}

void SubscriptionsModel::onSubscriptionAdded(ShvBrokerNodeItem *nd, const std::string &path)
{
	shvInfo() << nd->nodeId();
	beginResetModel();
	endResetModel();
}

QString SubscriptionsModel::method(int row) const
{
/*	if (m_shvTreeNodeItem.isNull()) {
		return QString();
	}
	return QString::fromStdString(m_shvTreeNodeItem->methods()[row].method);
*/
}

void SubscriptionsModel::onMethodsLoaded()
{
	loadRows();
	callGetters();
}

void SubscriptionsModel::onRpcMethodCallFinished(int method_ix)
{
/*	loadRow(method_ix);
	emitRowChanged(method_ix);
	emit methodCallResultChanged(method_ix);
*/
}

void SubscriptionsModel::emitRowChanged(int row_ix)
{
/*	QModelIndex ix1 = index(row_ix, 0);
	QModelIndex ix2 = index(row_ix, ColCnt - 1);
	emit dataChanged(ix1, ix2);
*/
}

void SubscriptionsModel::callGetters()
{
/*	for (unsigned i = 0; i < m_rows.size(); ++i) {
		const ShvMetaMethod *mm = metaMethodAt(i);
		if(mm) {
			if(mm->method == cp::Rpc::METH_GET || (mm->flags & cp::MetaMethod::Flag::IsGetter)) {
				callMethod(i);
			}
		}
	}
*/
}

/*const ShvMetaMethod *SubscriptionsModel::metaMethodAt(unsigned method_ix)
{
	if(method_ix >= m_rows.size() || m_shvTreeNodeItem.isNull())
		return nullptr;
	const QVector<ShvMetaMethod> &mm = m_shvTreeNodeItem->methods();
	if(method_ix >= static_cast<unsigned>(mm.size()))
		return nullptr;
	return &(mm[method_ix]);
}*/

void SubscriptionsModel::loadRow(unsigned method_ix)
{
/*	const ShvMetaMethod * mtd = metaMethodAt(method_ix);
	if(!mtd)
		return;
	RowVals &rv = m_rows[method_ix];
	shvDebug() << "load row:" << mtd->method << "flags:" << mtd->flags << mtd->flagsStr();
	rv[ColMethodName] = mtd->method;
	rv[ColSignature] = mtd->signatureStr();
	rv[ColFlags] = mtd->flagsStr();
	rv[ColAccessGrant] = mtd->accessGrant;
	rv[ColParams] = mtd->params;
	shvDebug() << "\t response:" << mtd->response.toCpon() << "is valid:" << mtd->response.isValid();
	if(mtd->response.isError()) {
		rv[ColResult] = mtd->response.error();
	}
	else {
		shv::chainpack::RpcValue result = mtd->response.result();
		rv[ColResult] = result;
	}
	rv[ColBtRun] = mtd->rpcRequestId;
	*/
}

void SubscriptionsModel::loadRows()
{
/*	m_rows.clear();
	if(!m_shvTreeNodeItem.isNull()) {
		const QVector<ShvMetaMethod> &mm = m_shvTreeNodeItem->methods();
		for (int i = 0; i < mm.count(); ++i) {
			RowVals rv;
			rv.resize(ColCnt);
			m_rows.push_back(rv);
			loadRow(m_rows.size() - 1);
		}
	}
	emit layoutChanged();
	emit reloaded();
	*/
}
