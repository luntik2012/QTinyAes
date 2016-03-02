#include "qtinyaes.h"
#include "aes.h"
#undef CBC
#undef ECB
#include <QtMath>

const qint32 QTinyAes::BLOCKSIZE = 16;
const QVector<quint32> QTinyAes::KEYSIZES = {16};

QTinyAes::QTinyAes(QObject *parent) ://TODO auto-generate key
	QObject(parent),
	cMode(CBC),
	mKey(),
	mIv()
{}

QTinyAes::QTinyAes(QTinyAes::CipherMode mode, const QByteArray &key, const QByteArray &iv, QObject *parent) :
	QObject(parent),
	cMode(mode),
	mKey(),
	mIv()
{
	this->setKey(key);
	this->setIv(iv);
}

QTinyAes::~QTinyAes()
{
	memset(this->mKey.data(), 0, this->mKey.size());
	memset(this->mIv.data(), 0, this->mIv.size());
}

QTinyAes::CipherMode QTinyAes::mode() const
{
	return this->cMode;
}

QByteArray QTinyAes::key() const
{
	return this->mKey;
}

QByteArray QTinyAes::iv() const
{
	return this->mIv;
}

void QTinyAes::setMode(QTinyAes::CipherMode mode)
{
	this->cMode = mode;
}

void QTinyAes::setKey(QByteArray key)
{
	Q_ASSERT_X(QTinyAes::KEYSIZES.contains(key.size()), Q_FUNC_INFO, "The Key-Length is not a valid length! (Check QTinyAes::KEYSIZES)");
	this->mKey = key;
}

void QTinyAes::resetKey()
{
	memset(this->mKey.data(), 0, this->mKey.size());
	this->mKey.clear();
}

void QTinyAes::setIv(QByteArray iv)
{
	if(iv.isEmpty())
		this->resetIv();
	else {
		Q_ASSERT_X(iv.size() >= QTinyAes::BLOCKSIZE, Q_FUNC_INFO, "The initialisation vector must be at least QTinyAes::BLOCKSIZE bytes long (or empty)");
		if(iv.size() > QTinyAes::BLOCKSIZE)
			qWarning("IV is longer then QTinyAes::BLOCKSIZE - the rest will be truncated");
		this->mIv = iv.mid(0, QTinyAes::BLOCKSIZE);
	}
}

void QTinyAes::resetIv()
{
	memset(this->mIv.data(), 0, this->mIv.size());
	this->mIv.clear();
}

QByteArray QTinyAes::encrypt(QByteArray plain) const
{
	Q_ASSERT_X(!this->mKey.isEmpty(), Q_FUNC_INFO, "The key must not be empty to encrypt data");
	preparePlainText(plain);
	QByteArray output(plain.size(), 0);

	switch(this->cMode) {
	case CBC:
		AES128_CBC_encrypt_buffer((uint8_t*)output.data(),
								  (uint8_t*)plain.data(),
								  (uint32_t)plain.size(),
								  (uint8_t*)this->mKey.data(),
								  (uint8_t*)(this->mIv.isEmpty() ? NULL : this->mIv.data()));
		break;
	case ECB:
	{
		int blockCount = plain.size() / QTinyAes::BLOCKSIZE;
		for(int i = 0; i < blockCount; i++) {
			AES128_ECB_encrypt((uint8_t*)plain.data() + (i * QTinyAes::BLOCKSIZE),
							   (uint8_t*)this->mKey.data(),
							   (uint8_t*)output.data() + (i * QTinyAes::BLOCKSIZE));
		}
		break;
	}
	default:
		Q_UNREACHABLE();
		return QByteArray();
	}

	return output;

}

QByteArray QTinyAes::decrypt(QByteArray cipher) const
{
	Q_ASSERT_X(!this->mKey.isEmpty(), Q_FUNC_INFO, "The key must not be empty to decrypt data");
	QByteArray output(cipher.size(), 0);

	switch(this->cMode) {
	case CBC:
		AES128_CBC_decrypt_buffer((uint8_t*)output.data(),
								  (uint8_t*)cipher.data(),
								  (uint32_t)cipher.size(),
								  (uint8_t*)this->mKey.data(),
								  (uint8_t*)(this->mIv.isEmpty() ? NULL : this->mIv.data()));
		break;
	case ECB:
	{
		int blockCount = cipher.size() / QTinyAes::BLOCKSIZE;
		for(int i = 0; i < blockCount; i++) {
			AES128_ECB_decrypt((uint8_t*)cipher.data() + (i * QTinyAes::BLOCKSIZE),
							   (uint8_t*)this->mKey.data(),
							   (uint8_t*)output.data() + (i * QTinyAes::BLOCKSIZE));
		}
		break;
	}
	default:
		Q_UNREACHABLE();
		return QByteArray();
	}

	if(restorePlainText(output))
		return output;
	else
		return QByteArray();
}

void QTinyAes::preparePlainText(QByteArray &data)
{
	quint32 dataLen = data.size();
	QByteArray preFill = QByteArray::number(dataLen, 16);
	preFill.prepend(QByteArray(8 - preFill.size(), '0'));
	data.prepend(preFill);
	data.append(QByteArray(QTinyAes::BLOCKSIZE - (data.size() % QTinyAes::BLOCKSIZE), 0));
}

bool QTinyAes::restorePlainText(QByteArray &data)
{
	bool ok;
	qint32 realLength = data.mid(0, 8).toInt(&ok, 16);
	if(ok) {
		data = data.mid(8, realLength);
		return data.size() == realLength;
	} else
		return false;
}
