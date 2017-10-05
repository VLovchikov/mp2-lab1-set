// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len<0)
		throw "NEGATIVE_SIZE";
	BitLen = len;
	MemLen = BitLen / (sizeof(TELEM) * 8) + 1;
	pMem = new TELEM[MemLen];
	if (pMem != 0)
		for (int i = 0; i<MemLen; i++)
			pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	if (pMem != 0)
		for (int i = 0; i< MemLen; i++)
			pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete[] pMem;
	pMem = NULL;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if (n>-1 && n<BitLen)
		return n / (sizeof(TELEM) * 8);
	else
		throw "GETMEMINDEX TOO SMALL OR TOO LARGE INDEX";
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	int len = n % (sizeof(TELEM) * 8);
	TELEM tmp = 1;
	tmp <<= len;
	return tmp;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if ((n>-1) && (n<BitLen))
	{
		int index = GetMemIndex(n);
		TELEM mask = GetMemMask(n);
		pMem[index] |= mask;
	}
	else
		throw "SETBIT TOO SMALL OR TOO LARGE INDEX";
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if ((n>-1) && (n<BitLen))
	{
		int index = GetMemIndex(n);
		TELEM mask = ~GetMemMask(n);
		pMem[index] &= mask;
	}
	else
		throw "CLRBIT TOO SMALL OR TOO LARGE INDEX";
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if ((n>-1) && (n<BitLen))
	{
		int index = GetMemIndex(n);
		TELEM mask = GetMemMask(n);
		TELEM tmp = pMem[GetMemIndex(n)];
		if (pMem[GetMemIndex(n)] == (tmp |= GetMemMask(n)))
			return 1;
		else
			return 0;
	}
	else
		throw "GETBIT TOO SMALL OR TOO LARGE INDEX";
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (this != &bf)
	{
		BitLen = bf.BitLen;
		if (MemLen != bf.MemLen)
		{
			MemLen = bf.MemLen;
			if (pMem != NULL) delete[]pMem;
			pMem = new TELEM[MemLen];
		}
		if (pMem != NULL)
			for (int i = 0; i<MemLen; i++)
				pMem[i] = bf.pMem[i];
	}
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if ((BitLen == bf.BitLen) && (MemLen == bf.MemLen))
	{
		int tmp = 1;
		for (int i = 0; i<MemLen; i++)
		{
			if (pMem[i] == bf.pMem[i])
				tmp = 1;
			else
			{
				tmp = 0;
				break;
			}
			return tmp;
		}
	}
	else
		return 0;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	int tmp = 0;
	if (BitLen != bf.BitLen)
		tmp = 1;
	else
		for (int i = 0; i<MemLen; i++)
			if (pMem[i] != bf.pMem[i])
			{
				tmp = 1;
				break;
			}
	return tmp;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int len;
	if (BitLen>bf.BitLen)
		len = BitLen;
	else
		len = bf.BitLen;
	TBitField tmp(len);
	for (int i = 0; i<MemLen; i++)
		tmp.pMem[i] = pMem[i];
	for (int i = 0; i<bf.MemLen; i++)
		tmp.pMem[i] |= bf.pMem[i];
	return tmp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int len;
	if (BitLen>bf.BitLen)
		len = BitLen;
	else
		len = bf.BitLen;
	TBitField tmp(len);
	for (int i = 0; i<MemLen; i++)
		tmp.pMem[i] = pMem[i];
	for (int i = 0; i<bf.MemLen; i++)
		tmp.pMem[i] &= bf.pMem[i];
	return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField tmp(BitLen);
	for (int i = 0; i<MemLen; i++)
		tmp.pMem[i] = ~pMem[i];
	return tmp;
}

// ввод/вывод

istream &operator >> (istream &istr, TBitField &bf) // ввод
{
	int i = 0;
	char ch;
	do { istr >> ch; } while (ch != ' ');
	while (1)
	{
		istr >> ch;
		if (ch == '0')
			bf.ClrBit(i++);
		else
			if (ch == '1')
				bf.SetBit(i++);
			else
				break;
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i<bf.GetLength(); i++)
	{
		if (bf.GetBit(i))
			ostr << 1;
		else ostr << 0;
	}
	return ostr;
}



