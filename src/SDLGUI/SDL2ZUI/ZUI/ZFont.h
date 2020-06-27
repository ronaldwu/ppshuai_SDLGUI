#ifndef __ZUIFONT_H__
#define __ZUIFONT_H__

#include "ZCommon.h"
#include "ZUtils.h"
#include "ZString.h"
#include "../base/time.h"
#include <map>

namespace ZuiLib
{
	class ZFont;
	class ZRenerder;

	struct TextStyle 
	{
		bool underline;
		bool bold;
		bool italic;
		color_t color;
	};

	//���ֻ�ͼ����
	class ZTextCache
	{
	public:
		ZTextCache();
		~ZTextCache();

		bool Init(ZFont* font,ZRenerder* ren,const char* text,const TextStyle* style);
		ZTextCache* Next(){return m_next;}
		void Mark();//���


		TextStyle* GetStyle(){return &m_style;}
		ZRenerder* GetRenerder(){return m_ren;}
		SDL_Texture* GetTexture(){return m_cacheTexture;}
	private:
		friend class ZFont;
		uint32_t m_mark;//�����ϴλ�ͼʱ��
		TextStyle m_style;
		ZTextCache* m_next;
		SDL_Texture* m_cacheTexture;
		ZRenerder* m_ren;
		static int s_count;
	};


	class ZFont:public RefCountedBase
	{
	public:
		enum FontStyle
		{
			FS_NORMAL=TTF_STYLE_NORMAL,
			FS_BOLD=TTF_STYLE_BOLD,
			FS_ITALIC=TTF_STYLE_ITALIC,
			FS_UNDERLINE=TTF_STYLE_UNDERLINE,
			FS_STRIKETHROUGH=TTF_STYLE_STRIKETHROUGH
		};

		enum FontHinting{
			FH_NORMAL=TTF_HINTING_NORMAL,
			FH_LIGHT=TTF_HINTING_LIGHT,
			FH_MONO=TTF_HINTING_MONO,
			FH_NONE=TTF_HINTING_NONE
		};



		ZFont(void);
		~ZFont(void);

		static RefCountedPtr<ZFont> Create()
		{
			return new RefCountedObject<ZFont>();
		}

		bool OpenFont(const char* file,int height);
		bool OpenFont(const char* file,int height,long index);


		int GetStyle();
		void SetStyle(int style);

		int GetOutline();
		void SetOutline(int outline);

		FontHinting GetHinting();
		void SetHinting(FontHinting hinting);

		int Height();

		int Ascent();
		int FontDescent();
		int LineSkip();
		int GetKerning();
		void SetKerning(int allowed);
		int FaceNumber();

		bool TextSize(const char* text,ZSize &sz);//utf8
		bool TextSize(const uint16_t* text,ZSize &sz);//unicode


		ZTextCache* GetTextCache(ZRenerder* ren,const char* text,const TextStyle* style);//�����ı�����
		void CollectTextCache(uint32_t life);//�ռ����ֻ���
		void FreeTextCache();//�ͷ����ֻ���

		bool CloseFont();

		TTF_Font* GetTTF(){return m_font;}
	private:
		friend class ZRenerder;
		TTF_Font* m_font;
		ZHashMap m_textCaches;
	};


	typedef RefCountedPtr<ZFont> ZFontPtr;


	//������Ϣ
	struct ZFontInfo
	{
		ZString name;
		int size;
		ZFontPtr font;
	};

	//�ı���Ϣ
	struct ZTextInfo
	{
		ZTextInfo()
			:size(13),underline(false),bold(false),italic(false)
		{

		}

		ZTextInfo(const ZTextInfo& other)
			:name(other.name),size(other.size),underline(other.underline),bold(other.bold),italic(other.italic)
		{

		}

		ZTextInfo& operator=(const ZTextInfo& other)
		{
			name=other.name;
			size=other.size;
			underline=other.underline;
			bold=other.bold;
			italic=other.italic;
			return *this;
		}

		ZString name;
		int size;
		bool underline;
		bool bold;
		bool italic;
	};



	//���������
	class ZFontMgr
	{
	public:
		ZFontMgr();
		~ZFontMgr();

		//��ȡ���壬ֱ�ӷ����Ѽ��ص����壬���û�м��س��Լ���
		ZFontPtr GetFont(const char* name,int size);

		//��������
		bool LoadFont(ZFontInfo* font);

		//�����ı���Ϣ
		bool SetTextInfo(const tinyxml2::XMLElement *elem);

		//��ȡ�ı���Ϣ
		ZTextInfo* GetTextInfo(const char* name);

		//Ĭ������
		static ZTextInfo* GetDefaultTextInfo(){return &s_defaultFont;}
		static void SetDefaultTextInfo(ZTextInfo* info){s_defaultFont=*info;}

		void CollectTextCache(uint32_t life);
	private:
		typedef std::vector<ZFontInfo> container_t;
		typedef std::map<ZString,ZTextInfo> map_t;

		static ZTextInfo s_defaultFont;//Ĭ������
		container_t m_fonts;//���ص�����
		map_t m_textInfos; //����������Ϣ����nameΪkey����
	};












}//namespace




#endif