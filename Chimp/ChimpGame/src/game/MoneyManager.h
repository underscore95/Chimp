#pragma once

#include "stdafx.h"

class MoneyManager {
public:
	DISABLE_COPY(MoneyManager);
	MoneyManager();
	~MoneyManager() = default;

	// Add or remove money from the player's balance, can go below 0
	void AddMoney(int amount);
	void RemoveMoney(int amount);

	// Get current balance
	[[nodiscard]] int GetMoney() const;

	// Check if current balance is greater than or equal to the amount
	[[nodiscard]] bool HasMoney(int amount) const;

	// Remove money if current balance is greater than or equal to the amount and return true.
	// else do nothing and return false.
	[[nodiscard]] bool SafeRemove(int amount);

	// Get the income per 6s
	[[nodiscard]] int GetIncome() const;

	// Add or remove income
	void AddToIncome(float amount);
	void RemoveFromIncome(float amount);

	void Update();

public:
	constexpr static int StartingMoney = 350;
	constexpr static int StartingIncome = 100;
	constexpr static float IncomeInterval = 8.0f;
private:
	int Money = StartingMoney;
	float Income = StartingIncome;
	Chimp::Timer m_IncomeTimer;
};