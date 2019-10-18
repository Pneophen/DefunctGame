class BaseObj
{
	private:
		virtual void VF(void) = 0;
		static unsigned int o_id;//universal id for objects
		unsigned int u_id;//unique id for obj
		

	protected:
		double x, y;//axis
		double vx, vy;//velocity
		int sx, sy;//sprite offset
		double px, py; //previous x and y positions
		int tx, ty; //x, y pos based on tile coordinates
		double time;
		short state_no;
		bool ctrl;
		short life;
		short muteki; //invincible
		bool alive;
		double angle;
		double speed;
		char physics;//a = air, g = ground, n = none
		char state_type;//s = stand, c = crouch, a = air
		char move_type;//a = attacking, h = getting hit, n = neutral
		double grav, fric;
		char facing, prev_facing;
		bool pause;//whether object is in a pause state
		char pause_type; //what type of pause
		bool visible;
		Uint32 color;
		Uint8 alpha;
		bool additive;
		double x_scale, y_scale;

		FIXED_ANIM anim;
		P_RECT clsn;
	
	public:

		BaseObj();
		virtual ~BaseObj();

		void SetPos(double, double);
		void SetVel(double, double);
		void SetVel(int, double); //set vel by speed & angle
		void SetSprOff(int, int);
		void SetPrevPos(void);
		void SetTilePos(int, int);
		void SetTime(double);
		void SetStateNo(short);
		void SetStateNo(short, bool);
		void SetCtrl(bool);
		void SetLife(short);
		void SetInv(short);
		void SetAngle(double);
		void SetSpeed(double);
		void SetPhysics(double, double);// grav and fric
		void SetFacing(char);
		void SetPause(bool);
		void SetVisible(bool);
		void SetClsn(short, short, unsigned short, unsigned short);
		void SetAnim(unsigned short, unsigned short, unsigned short);
		void SetAlive(bool);
		void SetColor(Uint8, Uint8, Uint8);
		void SetColor(Uint32);
		void SetAlpha(Uint8);
		void SetAdditive(bool);
		void SetScale(double, double);

		double GetX(void) const;
		double GetY(void) const;
		double GetVX(void) const;
		double GetVY(void) const;
		int GetSX(void) const;
		int GetSY(void) const;
		double GetPX(void) const;
		double GetPY(void) const;
		int GetTX(void) const;
		int GetTY(void) const;
		double GetTime(void) const;
		int GetIntTime(void) const;
		short GetStateNo(void) const;
		bool GetCtrl(void) const;
		short GetLife(void) const;
		short GetInv(void) const;
		double GetAngle(void) const;
		double GetSpeed(void) const;
		double GetGrav(void) const;
		double GetFric(void) const;
		char GetFacing(void) const;
		bool GetPause(void) const;
		char GetPauseType(void) const;
		bool GetVisible(void) const;
		FIXED_ANIM GetAnim(void) const;
		int GetAnimData(char) const;//gets animation data
		SDL_Rect GetAnimData(void) const; //gets animation for rendering
		P_RECT GetClsn(void) const;
		unsigned int GetUId(void) const;
		Uint32 GetColor(void) const;
		Uint8 GetAlpha(void) const;
		bool GetAdditive(void) const;
		bool IsAlive(void) const;
		double GetXScale(void) const;
		double GetYScale(void) const;

		void MoveObj(void);//
		void CalcAngle(void);//find objects angle by velocity
		void CalcSpeed(void);//find objects speed by velocity
		void AnimInc(void);
		void TimeInc(void);
		void InvInc(void);
		void AdjClsn(unsigned short, unsigned short);
		void ClearAnim(void);
		
		//BaseObj *link; nope
};

//-----------------------------------------------------------------------------
class Camera
{
	private:
		char follow;
		double x, y;

	public:
		SDL_Rect scene;

		Camera();
		void SetFollow(double);
		double GetX(void) const;
		double GetY(void) const;
		void SetX(double);
		void SetY(double);
};
//-----------------------------------------------------------------------------
class Vfx : public BaseObj
{
	private:
		short id;
		unsigned short spr_id;//index in sprite array
		int n_id; //id of npc that it belongs to
		bool back;//draw effect behind objects
		Uint8 alpha;
		int ex[5];

		void VF(void){};

	public:
		Vfx();
		~Vfx();

		void Init(int);
		void Move(const Camera&);
		void SetId(short);
		void SetNId(int);
		void States(double, double);
		bool GetBack(void) const;
		void DrawVfx(SURFACES*, int, int);

		//Vfx *link;
}; //visual effects
//-----------------------------------------------------------------------------
class VfxHead
{
	private:
		int count;

	public:
		std::list<Vfx> v_list;

		VfxHead();
		~VfxHead();
		
		int GetCount(void) const;
		void Add(int, int,  double, double, int);
		void DecCount(void);
		void KillAll(void);
		void DrawVfxs(SURFACES*, Camera&);
		void DrawVfxsBack(SURFACES*, Camera&);
};
//-----------------------------------------------------------------------------
class ObjManFx
{
    private:
        std::list<OBJ_DATA> vfx;
        std::list<OBJ_DATA> proj;

    public:
        ObjManFx();
        ~ObjManFx();

        void AddVfx(int, int, double, double, int);
        void AddProj(int, int, double, double, int, double);
        void ClearVfx(void);
        void ClearProj(void);
		int SizeProj(void);
		int SizeVfx(void);
		std::list<OBJ_DATA>::iterator ProjIt(bool);
		std::list<OBJ_DATA>::iterator VfxIt(bool);

};//object manager for proj/fx creation
//-----------------------------------------------------------------------------
class Projectile : public BaseObj
{
	private:
		short id;
		unsigned short spr_id;//index in sprite array
		int n_id; //id of npc that it belongs to
		short damage;//how much damage to inflict on npc
		bool can_absorb;
		bool blur;
		int ex[5];//extra variables

		void VF(void){};

	public:
		Projectile();
		~Projectile();

		void Init(int, double);
		void Move(const Camera&);
		void SetId(short);
		void SetNId(int);
		void States(ObjManFx&, AUDIO&);
		void DrawProjectile(SURFACES*, int, int);
		short GetId(void) const;
		bool CanAbsorb(void) const;
		short GetDamage(void) const;

}; //projectile object
//-----------------------------------------------------------------------------
class ScreenOffset
{
	public:
		int off_x;//offset
		int off_y;
		int shake_x;//shaking screen offset independent from screen offset
		int shake_y;
		int scale; //bitwise multiplication used
		double spd; //for fast slow motion

		ScreenOffset();
};
//-----------------------------------------------------------------------------
struct SFX_STRIP
{
    int x, y;
    char dir;
    SDL_Rect str;
};
//-----------------------------------------------------------------------------
class ScreenFx
{
	private:
		int x, y; //origin
		SFX_STRIP *h_str;//change to c++ list
		SFX_STRIP *v_str;//ditto
		SDL_Surface *spr;
		int factor;
		int time;
		bool vert;
		bool on;
		char type;

		
	public:
		ScreenFx();
		~ScreenFx();
		
		ScreenFx *link;
};
//-----------------------------------------------------------------------------
class ProjectileHead //head of projectile list
{
	private:
		int count;
		
	public:
		std::list<Projectile> p_list;

		ProjectileHead();
		~ProjectileHead();

		int GetCount(void) const;
		void Add(int, int, double, double, int, double);
		void DecCount(void);
		void KillAll(void);
		void DrawProjectiles(SURFACES*, Camera&); 
};
//-----------------------------------------------------------------------------
class Player : public BaseObj
{
	private:
		
		short freq;//fire rate
		short dash;
		short gun_lvl;
		int gun_xp;
		short gun_next;
		short pow;
		bool blur;
		bool super; // can super shot
		double b_time; //timer for bonus shot
		double b_shot; // the sweet spot
		int got_bshot; // got bonus shot
		unsigned char d_type; //level of dash
		unsigned short chain; // chain count
		short c_time; //time til chain counter resets
		unsigned short hits; // bullet hits on enemies
		short h_time; //time til hit counter resets
		bool max_chain; // hit max chain status
		int absorbed;
		bool allow_spawn;
		int score;

		void VF(void){};

	public:
		Player();
		~Player();

		void DrawPlayer(SURFACES*, Camera&);
		void SetState(short, bool);
		void ScoreAdd(unsigned int);
		void Init(const SPRITE&);
		void Move(const Camera&);
		void CmdCheck(ObjManFx&, short&);
		void ChainAdd(void);
		void HitAdd(void);
		short GetPow(void) const;
		bool AllowSpawn(void) const;
		void PlayerStates(ObjManFx&, short&, AUDIO&);
		void Action1(AUDIO&);
		void GotHit(int);
		void BulletAbsorb(void);
		void PowerAdd(short);
		unsigned short GetChain(void) const;
		short GetCTime(void) const;
		int GetScore(void) const;
		short GetDash(void) const;
		double GetBShot(void) const;
		double GetBTime(void) const;
};
//-----------------------------------------------------------------------------
class Npc : public BaseObj
{
	private:
		short id;//npc type
		unsigned short spr_id;//index in sprite array
		int n_id; //unique npc id
		char level;//diffculty level
		double mult; //multiplier
		bool blur;
		bool hard;
		int ex[5];//extra variables

		void VF(void){};

	public:
		Npc();
		~Npc();

		void SetId(short);
		void SetNId(int);
		void SetLevel(char);
		void SetEx(int, int);
		short GetId(void) const;
		char GetLevel(void) const;

		void Init(bool);
		void DrawNpc(bool, SURFACES*, int, int);
		void Move(const Camera&);
		bool Clsn(ProjectileHead&);
		void Ai(Player&);
		
		void States(Player&, ObjManFx&, AUDIO&);
		void Zoomer(Player&, ObjManFx&, AUDIO&);
		void PartTime(Player&, ObjManFx&, AUDIO&);
		void BigShip(Player&, ObjManFx&, AUDIO&);
		void Brick(Player&, ObjManFx&, AUDIO&);
		void Leech(Player&, ObjManFx&, AUDIO&);
		void Mine(Player&, ObjManFx&, AUDIO&);
		void Formation(Player&, ObjManFx&, AUDIO&);
		void SquareWave(Player&, ObjManFx&, AUDIO&);
		void Wall(Player&, ObjManFx&, AUDIO&);
		void ZigZag(Player&, ObjManFx&, AUDIO&);
		void Choppa(Player&, ObjManFx&, AUDIO&);
		void Diamond(Player&, ObjManFx&, AUDIO&);
		void KillScreen(Player&, ObjManFx&, AUDIO&);
		void PowUp(Player&, ObjManFx&, AUDIO&);

		//Npc *link;
}; //main npc object
//-----------------------------------------------------------------------------
class NpcHead //head of npc list
{
	private:
		int count;//npcs in existance(related to list)
		int id;//unique id(based on npc count but doesn't reset)
		short time;//time before respawn
		char big;//big ship limit
		char zoomer;//
		char part_time;//
		char brick;//
		char mine;//
		char form;
		char square;
		char wall;
		char zig;
		char choppa;
		char diamond;
		unsigned char wave;
		int enemy;//npcs in existance(related to wave)
		int en_max;//max npcs allowable
		char level;//difficulty level
		bool hard;//not normal mode
		bool can_spawn;
		
	public:
		std::list<Npc> n_list;

		NpcHead();
		~NpcHead();

		void Add(int, double, double);
		//void Delete(Npc*, Npc*);
		void KillAll(void);
		void IncEnemyType(int);
		void DecEnemyType(int);
		void CountAdd(int);
		void EnemyAdd(int);
		int GetCount(void) const;//return enemy object count
		int GetEnemy(void) const;//return total enemies
		char GetEnemyCount(int) const; //return count of specific enemy type
		char GetLevel(void) const;
		void Advance(void);
		void SetSpawn(bool);
		bool CanSpawn(void) const;
		void AddCount(int);
		void AddId(int);
		void SetId(int);
		int GetId(void) const;
		void IncTime(void);
		short GetTime(void) const;
		void SetTime(short);
		unsigned char GetWave(void) const;
		void DrawNpcs(SURFACES*, Camera&, bool);
		void NpcSpawn(void);

		//Npc *link;//start of npc list
};
//-----------------------------------------------------------------------------
class Hud
{
	private:
		int score;
		short life;
		short dash;
		short pow;
		unsigned short chain;
		short c_time;
		short enemy;
		unsigned char wave;
		bool super;
		Uint8 alpha;
		int b_shot;
		int b_time;
		int counter_space;

	public:
		Hud();
		
		void GetPlayerData(Player&);
		void GetEnemyData(NpcHead&);
		void AlphaCheck(double);
		void DrawText(SURFACES&, FONT*);
		void DrawHud(SURFACES&, FONT*);
		void DrawBars(SURFACES&);
		void DrawCombo(SURFACES&, FONT*);
		void HudState(void);
};
//-----------------------------------------------------------------------------
class BgObj//non collision scrolling background objs
{
	private:
		FIXED_ANIM anim; //
		int time;
		double x, y; //pos based on room
		double cx, cy; //pos based on camera
		int w, h; //spacing width and height for tiling
		double hs, vs;//horizontal speed, vertical speed
		char t_type, s_type;//tile type, scroll type
		bool at;//automatic scrolling?
		int scene;//level
		int layer;
		P_LINE *line;
		
	public:
		BgObj();
		
		void SetLineList(int);//how many lines to set up
		void SetLine(int, int, int, double, int, int, Uint32);
		void SetLayer(int);
		void DrawBgA(SURFACES*, int, int);
		void DrawBg(SURFACES*, int, int);
		int GetSceneNum(void);
};
//-----------------------------------------------------------------------------
class BgHead
{
	
	public:
		std::list<BgObj> b_list;

		void BgGenA(int);// top/bottom layer
		//void BgGenB(void);//mid layer
		//void BgGenC(void);//bottom layer

		void BgHead::DrawBgs(SURFACES*, Camera&);

		BgHead();
};
//-----------------------------------------------------------------------------
struct OBJECTS
{
    Player p1;
	NpcHead n;
    ProjectileHead p;
    VfxHead v;
    Camera cam;
	Hud h;
	BgHead b;
	int map[MAPX][MAPY];
    //ScreenFx s;
}; //all objects

