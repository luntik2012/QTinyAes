#include <QString>
#include <QtTest>
#include <qtinyaes.h>

#define TEST_ROUNDS 256

class QTinyAesTest : public QObject
{
	Q_OBJECT

public:
	QTinyAesTest();

private Q_SLOTS:
	void initTestCase();

	void testGenerateKey();

	void testCTR_data();
	void testCTR();
	void testCBC_data();
	void testCBC();
	void testECB_data();
	void testECB();

private:
	QByteArray generateData(int size);

	void data();
	void test(QTinyAes::CipherMode mode);

	QTinyAes aes;
};

QTinyAesTest::QTinyAesTest(){}

void QTinyAesTest::initTestCase()
{
	qsrand(QDateTime::currentMSecsSinceEpoch());
}

void QTinyAesTest::testGenerateKey()
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
	QTinyAes aes(QTinyAes::CTR,
				 QTinyAes::generateKey(),
				 generateData(QTinyAes::BlockSize));

	auto plain = generateData(42);
	auto cipher = aes.encrypt(plain);
	auto result = aes.decrypt(cipher);
	QCOMPARE(plain, result);
#else
	qDebug("Not implemented before Qt 5.10");
#endif
}

void QTinyAesTest::testCTR_data()
{
	data();
}

void QTinyAesTest::testCTR()
{
	test(QTinyAes::CTR);
}

void QTinyAesTest::testCBC_data()
{
	data();
}

void QTinyAesTest::testCBC()
{
	test(QTinyAes::CBC);
}

void QTinyAesTest::testECB_data()
{
	data();
}

void QTinyAesTest::testECB()
{
	test(QTinyAes::ECB);
}

QByteArray QTinyAesTest::generateData(int size)
{
	QByteArray data(size, Qt::Uninitialized);
	for(int i = 0; i < size; i++)
		data[i] = (char)qrand();
	return data;
}

void QTinyAesTest::data()
{
	QTest::addColumn<QByteArray>("plain");
	QTest::addColumn<QByteArray>("key");
	QTest::addColumn<QByteArray>("iv");

	for(int i = 0; i < TEST_ROUNDS; i++) {
		QTest::newRow(qPrintable(QStringLiteral("size_%1").arg(i)))
				<< generateData(i * i)
				<< generateData(QTinyAes::KeySize)
				<< generateData(QTinyAes::BlockSize);
	}
}

void QTinyAesTest::test(QTinyAes::CipherMode mode)
{
	QFETCH(QByteArray, plain);
	QFETCH(QByteArray, key);
	QFETCH(QByteArray, iv);

	this->aes.setMode(mode);
	this->aes.setKey(key);
	this->aes.setIv(iv);

	auto cipher = aes.encrypt(plain);
	auto result = aes.decrypt(cipher);

	QCOMPARE(plain, result);
}

QTEST_APPLESS_MAIN(QTinyAesTest)

#include "tst_qtinyaestest.moc"
