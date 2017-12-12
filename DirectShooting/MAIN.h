//ヘッダーファイルのインクルード
#include <windows.h>
#include <d3d11.h>
#include <d3dx10.h>
#include <d3dx11.h>
#include <d3dCompiler.h>
#include <vector>
//必要なライブラリファイルのロード
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")
//警告非表示
#pragma warning(disable : 4305)
#include "Constant.h"
float M_PI = acos(-1);
//マクロ
#define SAFE_RELEASE(x) if(x){x->Release(); x;}

//頂点の構造体
struct SimpleVertex
{
	D3DXVECTOR3 Pos; //位置
	D3DXVECTOR4 Color; //位置
	D3DXVECTOR4 dx; // 頂点のズレ
};
//Simpleシェーダー用のコンスタントバッファーのアプリ側構造体 もちろんシェーダー内のコンスタントバッファーと一致している必要あり
struct SIMPLESHADER_CONSTANT_BUFFER
{
	D3DXMATRIX mWVP;
	D3DXVECTOR4 vColor;
};
//物体の構造体
struct MODEL//このサンプルではプリン
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR4 vColor;
	D3DXVECTOR4 dx[PUDDING_REF_NUMV];
	D3DXVECTOR3 v[PUDDING_REF_NUMV];
};
//弾の構造体
struct BULLET
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 v;
	bool alive;
	int time;
};

//
//
//
class MAIN
{
public:
	MAIN();
	~MAIN();
	HRESULT InitWindow(HINSTANCE,INT,INT,INT,INT,LPCWSTR);
	LRESULT MsgProc(HWND,UINT,WPARAM,LPARAM);
	HRESULT InitD3D();
	HRESULT InitPolygon();
	HRESULT InitShader();

	void Loop();
	void App();
	void Update();
	void Render();
	void DestroyD3D();
	void UpdatePuddingForm();

	std::vector<SimpleVertex> makePuddingVertices();
	std::vector<WORD> makePuddingIndices();
	std::vector<SimpleVertex> makeBulletVertices();
	std::vector<WORD> makeBulletIndices();

	//↓アプリにひとつ
	HWND m_hWnd;
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	IDXGISwapChain* m_pSwapChain;
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11DepthStencilView* m_pDepthStencilView;
	ID3D11Texture2D* m_pDepthStencil;
	//↓モデルの種類ごと(モデルの構造が全て同一ならアプリにひとつ）
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11Buffer* m_pConstantBuffer;
	//↓モデルごと	
	ID3D11Buffer* m_pBulletVertexBuffer;
	ID3D11Buffer* m_pBulletIndexBuffer;
	ID3D11Buffer* m_pPuddingVertexBuffer;
	ID3D11Buffer* m_pPuddingIndexBuffer;
	std::vector<SimpleVertex> m_puddingVertices;
	std::vector<WORD> m_puddingIndices;
	std::vector<SimpleVertex> m_bulletVertices;
	std::vector<WORD> m_bulletIndices;
	//モデルのインスタンス配列
	MODEL m_Model[MAX_MODEL];
	BULLET m_BulletModel[MAX_BULLET_NUM];
	int m_iNumModel;

	//カメラ関係
	D3DXVECTOR3 m_campos;
	D3DXVECTOR3 m_camlook;
	float m_cam_vtheta;
};
