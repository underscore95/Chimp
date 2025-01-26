#pragma once

#include "ServerMatch.h"
#include "stdafx.h"

// Represents a set of matches, wraps multiple unordered maps for quick lookups
class MatchSet {
public:
	MatchSet() = default;
	virtual ~MatchSet() = default;

	void AddMatch(const ServerMatch &match);

    void RemoveMatch(const ServerMatch& match);

    // Remove a match, returns a copy of the removed match
    ServerMatch RemoveMatchById(int matchId);

	Chimp::OptionalReference<ServerMatch> GetMatchByPlayerId(int playerId);

	Chimp::OptionalReference<ServerMatch> GetMatchByMatchId(int matchId);

	// ITERATOR
    class Iterator {
    public:
        Iterator(std::unordered_map<int, ServerMatch>::iterator inner) : m_Iter(inner) {}

        // Dereference to get only ServerMatch, not the pair
        ServerMatch& operator*() { return m_Iter->second; }
        ServerMatch* operator->() { return &m_Iter->second; }

        Iterator& operator++() {
            ++m_Iter;
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            ++m_Iter;
            return temp;
        }

        bool operator==(const Iterator& other) const { return m_Iter == other.m_Iter; }
        bool operator!=(const Iterator& other) const { return m_Iter != other.m_Iter; }

    private:
        std::unordered_map<int, ServerMatch>::iterator m_Iter;
    };

    Iterator begin() { return Iterator(m_Matches.begin()); }
    Iterator end() { return Iterator(m_Matches.end()); }

private:
	std::unordered_map<int, ServerMatch> m_Matches; // matchId -> match
	std::unordered_map<int, int> m_PlayerOneIdMatchId; // playerId -> matchId
	std::unordered_map<int, int> m_PlayerTwoIdMatchId; // playerId -> matchId
};