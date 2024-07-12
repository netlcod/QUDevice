#include "qumodbusmasterregistermodel.h"

RegisterModel::RegisterModel(QObject* parent) : QAbstractTableModel(parent), m_coilRegisters(1, false), m_holdingRegisters(1, 0u) {}

int RegisterModel::rowCount(const QModelIndex&) const {
    return m_registerCount;
}

int RegisterModel::columnCount(const QModelIndex&) const {
    return ColumnCount;
}

QVariant RegisterModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() >= m_registerCount || index.column() >= ColumnCount) {
        return QVariant();
    }

    if (index.column() == NumColumn && role == Qt::DisplayRole) {
        return QString::number(m_registerAddress + index.row());
    }

    if (index.column() == CoilsColumn && role == Qt::CheckStateRole) { // coils
        return m_coilRegisters.at(index.row()) ? Qt::Checked : Qt::Unchecked;
    }

    if (index.column() == HoldingColumn && role == Qt::DisplayRole) { // holding registers
        return QString("0x%1").arg(QString::number(m_holdingRegisters.at(index.row()), 16));
    }

    return QVariant();
}

QVariant RegisterModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case NumColumn:
                return QStringLiteral("#");
            case CoilsColumn:
                return QStringLiteral("Coils  ");
            case HoldingColumn:
                return QStringLiteral("Holding Registers");
            default:
                break;
        }
    }
    return QVariant();
}

bool RegisterModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!index.isValid() || index.row() >= m_registerCount || index.column() >= ColumnCount) {
        return false;
    }

    if (index.column() == CoilsColumn && role == Qt::CheckStateRole) { // coils
        auto s = static_cast<Qt::CheckState>(value.toUInt());
        s == Qt::Checked ? m_coilRegisters.setBit(index.row()) : m_coilRegisters.clearBit(index.row());
        emit dataChanged(index, index);
        return true;
    }

    if (index.column() == HoldingColumn && role == Qt::EditRole) { // holding registers
        bool result      = false;
        quint16 newValue = value.toString().toUShort(&result, 16);
        if (result) {
            m_holdingRegisters[index.row()] = newValue;
        }

        emit dataChanged(index, index);
        return result;
    }

    return false;
}

Qt::ItemFlags RegisterModel::flags(const QModelIndex& index) const {
    if (!index.isValid() || index.row() >= m_registerCount || index.column() >= ColumnCount) {
        return QAbstractTableModel::flags(index);
    }

    Qt::ItemFlags flags = QAbstractTableModel::flags(index);
    if (index.isValid()) {
        if (index.column() == CoilsColumn) { // coils
            return flags | Qt::ItemIsUserCheckable;
        }
        if (index.column() == HoldingColumn) { // holding registers
            return flags | Qt::ItemIsEditable;
        }
    }

    return flags;
}

QBitArray RegisterModel::getCoilRegisters() const {
    return m_coilRegisters;
}

QList<quint16> RegisterModel::getHoldingRegisters() const {
    return m_holdingRegisters;
}

void RegisterModel::setStartAddress(int address) {
    m_registerAddress = address;

    emit updateViewport();
}

void RegisterModel::setNumberOfValues(int number) {
    m_registerCount = number;

    beginRemoveRows(QModelIndex(), 0, rowCount());
    m_coilRegisters.clear();
    m_holdingRegisters.clear();
    endRemoveRows();

    beginInsertRows(QModelIndex(), 0, m_registerCount);
    m_coilRegisters.resize(number);
    m_holdingRegisters.resize(number);
    endInsertRows();

    emit updateViewport();
}
