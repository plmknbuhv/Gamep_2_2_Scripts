#pragma once
enum class Layer
{
	DEFAULT,
	BACKGROUND,
	SHADOW,
	PLAYER,
	ENEMY,
	ENEMY_PROJECTILE,
	BABY,
	PLAYER_PROJECTILE,
	EFFECT,
	UI,
	FADE,
	END
};

enum class PenType
{
	RED, GREEN, END
};

enum class BrushType
{
	HOLLOW, RED, GREEN, END
};

enum class FontType
{
	UI, TITLE, END
};

enum class PlayMode { Once, Loop, Counted };

enum class BossPhase { Phase1, Phase2 };

enum class PlayerType { Ranged, Melee };