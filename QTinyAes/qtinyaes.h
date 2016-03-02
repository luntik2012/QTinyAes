#ifndef QTINYAES
#define QTINYAES

#include <QObject>
#include <QByteArray>
#include <QVector>

class QTinyAes : public QObject
{
	Q_OBJECT

	Q_PROPERTY(CipherMode mode READ mode WRITE setMode)
	Q_PROPERTY(QByteArray key READ key WRITE setKey RESET resetKey)
	Q_PROPERTY(QByteArray iv READ iv WRITE setIv RESET resetIv)

public:
	enum CipherMode {
		CBC,
		ECB
	};
	Q_ENUM(CipherMode)

	static const qint32 BLOCKSIZE;
	static const QVector<quint32> KEYSIZES;

	QTinyAes(QObject *parent = NULL);
	QTinyAes(CipherMode mode, const QByteArray &key, const QByteArray &iv = QByteArray(), QObject *parent = NULL);
	~QTinyAes();

	CipherMode mode() const;
	QByteArray key() const;
	QByteArray iv() const;

	Q_INVOKABLE QByteArray encrypt(QByteArray plain) const;
	Q_INVOKABLE QByteArray decrypt(QByteArray cipher) const;

public slots:
	void setMode(CipherMode mode);
	void setKey(QByteArray key);
	void resetKey();
	void setIv(QByteArray iv);
	void resetIv();

private:
	CipherMode cMode;
	QByteArray mKey;
	QByteArray mIv;

	static void preparePlainText(QByteArray &data);
	static bool restorePlainText(QByteArray &data);
};

#endif // QTINYAES

