#include "../Manager/System/SceneManager.h"
#include "Utility.h"

int Utility::Round(float v)
{
    return static_cast<int>(roundf(v));
}

std::vector<std::string> Utility::Split(std::string& line, char delimiter)
{

    std::istringstream stream(line);
    std::string field;
    std::vector<std::string> result;
    
    while (getline(stream, field, delimiter)) {
        result.push_back(field);
    }

    return result;

}

double Utility::Rad2DegD(double rad)
{
    return rad * (180.0 / DX_PI);
}

float Utility::Rad2DegF(float rad)
{
    return rad * (180.0f / DX_PI_F);
}

int Utility::Rad2DegI(int rad)
{
    return rad * Round(180.0f / DX_PI_F);
}

double Utility::Deg2RadD(double deg)
{
    return deg * (DX_PI / 180.0);
}

float Utility::Deg2RadF(float deg)
{
    return deg * (DX_PI_F / 180.0f);
}

int Utility::Deg2RadI(int deg)
{
    return deg * Round(DX_PI_F / 180.0f);
}

double Utility::DegIn360(double deg)
{
    deg = fmod(deg, 360.0);
    if (deg < 0.0f)
    {
        deg += 360.0;
    }
    return deg;
}

double Utility::RadIn2PI(double rad)
{
    rad = fmod(rad, DX_TWO_PI);
    if (rad < 0.0)
    {
        rad += DX_TWO_PI;
    }
    return rad;
}

int Utility::DirNearAroundRad(float from, float to)
{

    float ret = 1.0f;

    float diff = to - from;

    if (diff >= 0.0f)
    {

        // 比較元よりも時計回りに位置する

        if (diff > DX_PI_F)
        {
            // でも、180度以上離れているので、反時計回りの方が近い
            ret = -1.0f;
        }
        else
        {
            // 時計回り
            ret = 1.0f;
        }

    }
    else
    {

        // 比較元よりも反時計回りに位置する

        if (diff < -DX_PI_F)
        {
            // でも、180度以上離れているので、時計回りの方が近い
            ret = 1.0f;
        }
        else
        {
            // 反時計回り
            ret = -1.0f;
        }

    }

    return static_cast<int>(ret);

}

int Utility::DirNearAroundDeg(float from, float to)
{

    float ret = 1.0f;

    float diff = to - from;

    if (diff >= 0.0f)
    {

        // 比較元よりも時計回りに位置する

        if (diff > 180.0f)
        {
            // でも、180度以上離れているので、反時計回りの方が近い
            ret = -1.0f;
        }
        else
        {
            // 時計回り
            ret = 1.0f;
        }

    }
    else
    {

        // 比較元よりも反時計回りに位置する

        if (diff < -180.0f)
        {
            // でも、180度以上離れているので、時計回りの方が近い
            ret = 1.0f;
        }
        else
        {
            // 反時計回り
            ret = -1.0f;
        }

    }

    return static_cast<int>(ret);

}

int Utility::Lerp(int start, int end, float t)
{
    // 線形補間
    if (t >= 1.0f)
    {
        return end;
    }

    int ret = start;
    ret += Round(t * static_cast<float>(end - start));
    return ret;
}

float Utility::Lerp(float start, float end, float t)
{
    // 線形補間
    if (t >= 1.0f)
    {
        return end;
    }

    float ret = start;
    ret += t * (end - start);
    return ret;
}

double Utility::Lerp(double start, double end, double t)
{
    // 線形補間
    if (t >= 1.0)
    {
        return end;
    }

    double ret = start;
    ret += t * (end - start);
    return ret;
}

Vector2 Utility::Lerp(const Vector2& start, const Vector2& end, float t)
{
    // 線形補間
    if (t >= 1.0f)
    {
        return end;
    }

    Vector2 ret = start;
    ret.x += Round(t * static_cast<float>((end.x - start.x)));
    ret.y += Round(t * static_cast<float>((end.y - start.y)));
    return ret;
}

VECTOR Utility::Lerp(const VECTOR& start, const VECTOR& end, float t)
{
    // 線形補間
    if (t >= 1.0f)
    {
        return end;
    }

    VECTOR ret = start;
    ret.x += t * (end.x - start.x);
    ret.y += t * (end.y - start.y);
    ret.z += t * (end.z - start.z);

    return ret;
}

double Utility::LerpDeg(double start, double end, double t)
{

    double ret;

    double diff = end - start;
    if (diff < -180.0)
    {
        end += 360.0;
        ret = Lerp(start, end, t);
        if (ret >= 360.0)
        {
            ret -= 360.0;
        }
    }
    else if (diff > 180.0)
    {
        end -= 360.0;
        ret = Lerp(start, end, t);
        if (ret < 0.0)
        {
            ret += 360.0;
        }
    }
    else
    {
        ret = Lerp(start, end, t);
    }

    return ret;

}

COLOR_F Utility::Lerp(const COLOR_F& start, const COLOR_F& end, float t)
{
    // 線形補間
    if (t >= 1.0f)
    {
        return end;
    }

    COLOR_F ret = start;
    ret.r += t * (end.r - start.r);
    ret.g += t * (end.g - start.g);
    ret.b += t * (end.b - start.b);
    ret.a += t * (end.a - start.a);
    return ret;
}

Vector2 Utility::Bezier(const Vector2& p1, const Vector2& p2, const Vector2& p3, float t)
{
    Vector2 a = Lerp(p1, p2, t);
    Vector2 b = Lerp(p2, p3, t);
    return Lerp(a, b, t);
}

VECTOR Utility::Bezier(const VECTOR& p1, const VECTOR& p2, const VECTOR& p3, float t)
{
    VECTOR a = Lerp(p1, p2, t);
    VECTOR b = Lerp(p2, p3, t);
    return Lerp(a, b, t);
}

VECTOR Utility::RotXZPos(const VECTOR& centerPos, const VECTOR& radiusPos, float rad)
{
    float x = ((radiusPos.x - centerPos.x) * cosf(rad)) - ((radiusPos.z - centerPos.z) * sinf(rad));
    float z = ((radiusPos.x - centerPos.x) * sinf(rad)) + ((radiusPos.z - centerPos.z) * cosf(rad));
    return VGet(centerPos.x + x, radiusPos.y, centerPos.z + z);
}

double Utility::Magnitude(const Vector2& v)
{
    return sqrt((v.x * v.x) + (v.y * v.y));
}

double Utility::Magnitude(const VECTOR& v)
{
    return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

float Utility::MagnitudeF(const VECTOR& v)
{
    return sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

int Utility::SqrMagnitude(const Vector2& v)
{
    return v.x * v.x + v.y * v.y;
}

float Utility::SqrMagnitudeF(const VECTOR& v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

double Utility::SqrMagnitude(const VECTOR& v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

double Utility::SqrMagnitude(const VECTOR& v1, const VECTOR& v2)
{
    return pow(v2.x - v1.x, 2) + pow(v2.y - v1.y, 2) + pow(v2.z - v1.z, 2);
}

double Utility::Distance(const Vector2& v1, const Vector2& v2)
{
    return sqrt(pow(v2.x - v1.x, 2) + pow(v2.y - v1.y, 2));
}

double Utility::Distance(const VECTOR& v1, const VECTOR& v2)
{
    return sqrt(pow(v2.x - v1.x, 2) + pow(v2.y - v1.y, 2) + pow(v2.z - v1.z, 2));
}

bool Utility::IsHitSpheres(const VECTOR& pos1, float radius1, const VECTOR& pos2, float radius2)
{
    // 球体同士の衝突判定
    bool ret = false;

    // お互いの半径の合計
    float radius = radius1 + radius2;

    // 座標の差からお互いの距離を取る
    VECTOR diff = VSub(pos2, pos1);

    // 三平方の定理で比較(SqrMagnitudeと同じ)
    float dis = (diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z);
    if (dis < (radius * radius))
    {
        ret = true;
    }

    return ret;
}

bool Utility::IsHitSphereCapsule(
    const VECTOR& sphPos, float sphRadius, 
    const VECTOR& capPos1, const VECTOR& capPos2, float capRadius)
{

    bool ret = false;

    // カプセル球体の中心を繋ぐベクトル
    VECTOR cap1to2 = VSub(capPos2, capPos1);

    // ベクトルを正規化
    VECTOR cap1to2ENor = VNorm(cap1to2);

    // カプセル繋ぎの単位ベクトルと、
    // そのベクトル元から球体へのベクトルの内積を取る
    float dot = VDot(cap1to2ENor, VSub(sphPos, capPos1));

    // 内積で求めた射影距離を使って、カプセル繋ぎ上の座標を取る
    VECTOR capRidePos = VAdd(capPos1, VScale(cap1to2ENor, dot));

    // カプセル繋ぎのベクトルの長さを取る
    float len = Utility::MagnitudeF(cap1to2);

    // 球体がカプセル繋ぎ上にいるか判別するため、比率を取る
    float rate = dot / len;

    VECTOR centerPos;

    // 球体の位置が３エリアに分割されたカプセル形状のどこにいるか判別
    if (rate > 0.0f && rate <= 1.0f)
    {
        // ①球体がカプセル繋ぎ上にいる
        centerPos = VAdd(capPos1, VScale(cap1to2ENor, dot));
    }
    else if (rate > 1.0f)
    {
        // ②球体がカプセルの終点側にいる
        centerPos = capPos2;
    }
    else if (rate < 0.0f)
    {
        // ③球体がカプセルの始点側にいる
        centerPos = capPos1;
    }
    else
    {
        // ここにきてはいけない
    }

    // 球体同士の当たり判定
    if (Utility::IsHitSpheres(centerPos, capRadius, sphPos, sphRadius))
    {
        ret = true;
    }
    else
    {
        ret = false;
    }

    return ret;

}

bool Utility::Equals(const VECTOR& v1, const VECTOR& v2)
{
    if (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z)
    {
        return true;
    }
    return false;
}

bool Utility::EqualsVZero(const VECTOR& v1)
{
    const VECTOR& v2 = VECTOR_ZERO;
    if (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z)
    {
        return true;
    }
    return false;
}

VECTOR Utility::Normalize(const Vector2& v)
{
    VECTOR ret = VGet(
        static_cast<float>(v.x),
        static_cast<float>(v.y),
        0.0f
    );
    float len = static_cast<float>(Magnitude(v));
    ret.x /= len;
    ret.y /= len;
    ret.z /= len;
    return ret;
}

VECTOR Utility::VNormalize(const VECTOR& v)
{
    if (Utility::EqualsVZero(v))
    {
        // Quaternion計算でゼロを渡して、
        // エラー(-1, -1, -1)が返ってくると困る
        return v;
    }
    return VNorm(v);
}

double Utility::AngleDeg(const VECTOR& from, const VECTOR& to)
{
    // sqrt(a) * sqrt(b) = sqrt(a * b) -- valid for real numbers
    auto fLen = SqrMagnitude(from);
    auto tLen = SqrMagnitude(to);
    auto denominator = sqrt(fLen * tLen);
    if (denominator < kEpsilonNormalSqrt)
    {
        return 0.0f;
    }

    //float dot = std::clamp(Dot(from, to) / denominator, -1.0f, 1.0f);
    //auto dot = Dot(from, to) / denominator;
    auto dot = VDot(from, to) / denominator;
    if (dot < -1.0f)
    {
        dot = -1.0f;
    }
    if (dot > 1.0f)
    {
        dot = 1.0f;
    }

    return acos(dot) * (180.0 / DX_PI);
}

void Utility::DrawLineDir(const VECTOR& pos, const VECTOR& dir, int color, float len)
{
    auto nDir = Utility::VNormalize(dir);
    auto sPos = VAdd(pos, VScale(nDir, -len));
    auto ePos = VAdd(pos, VScale(nDir, len));
    DrawLine3D(sPos, ePos, color);
    DrawSphere3D(ePos, 5.0f, 5, color, color, true);
}

void Utility::DrawLineXYZ(const VECTOR& pos, const MATRIX& rot, float len)
{

    VECTOR dir;

    // X
    dir = VTransform(Utility::DIR_R, rot);
    DrawLineDir(pos, dir, 0xff0000, len);

    // Y
    dir = VTransform(Utility::DIR_U, rot);
    DrawLineDir(pos, dir, 0x00ff00, len);

    // Z
    dir = VTransform(Utility::DIR_F, rot);
    DrawLineDir(pos, dir, 0x0000ff, len);

}

void Utility::DrawLineXYZ(const VECTOR& pos, const Quaternion& rot, float len)
{

    VECTOR dir;

    // X
    dir = rot.GetRight();
    DrawLineDir(pos, dir, 0xff0000, len);

    // Y
    dir = rot.GetUp();
    DrawLineDir(pos, dir, 0x00ff00, len);

    // Z
    dir = rot.GetForward();
    DrawLineDir(pos, dir, 0x0000ff, len);

}

bool Utility::IsTimeOver(float& totalTime, const float& waitTime)
{
    //デルタタイム
    auto delta = SceneManager::GetInstance().GetDeltaTime();
    totalTime += delta;

    //待機時間を超過しているか判断
    if (totalTime >= waitTime)
    {
        return true;
    }

    return false;
}

void Utility::DrawStringPlace(std::string _str, int _line, int _posY, int _color, STRING_PLACE _place)
{
    //文字列の長さを取得
    int width = GetDrawStringWidth(_str.c_str(), strlen(_str.c_str()));

    //表示するX座標を求める
    int posX = _line;
    switch (_place)
    {
        //左揃えの場合そのまま
    case Utility::STRING_PLACE::LEFT:
        break;
		//中央揃えの場合　文字列の長さの半分を引く
    case Utility::STRING_PLACE::CENTER:
		posX = _line - width / 2;
        break;
		//右揃えの場合　文字列の長さを引く
    case Utility::STRING_PLACE::RIGHT:
		posX = _line - width;
        break;
    default:
        break;
    }
    DrawString(posX, _posY, _str.c_str(), _color);
}

const VECTOR Utility::GetMoveVec(const VECTOR _start, const VECTOR _goal, const float _speed)
{
    //標的への方向ベクトルを取得
    VECTOR targetVec = VSub(_goal, _start);

    //正規化
    targetVec = VNorm(targetVec);

    //移動量を求める
    VECTOR ret = VScale(targetVec, _speed);

    return ret;
}

const VECTOR Utility::GetRotAxisToTarget(const VECTOR _pos, const VECTOR _targetPos, const VECTOR _needAxis)
{
    //対象までの方向ベクトル
    VECTOR targetVec = GetMoveVec(_pos, _targetPos);

    //回転量
    Quaternion turQuaRot = Quaternion::LookRotation(targetVec);

    //VECTOR変換
    VECTOR ret = turQuaRot.ToEuler();

    //必要ない軸は除去
    ret.x *= _needAxis.x;
    ret.y *= _needAxis.y;
    ret.z *= _needAxis.z;

   return ret;
}

void Utility::LookAtTarget(Transform& _trans, const VECTOR _toTargetAxis, const float _time, const VECTOR _relativePos)
{
    //しきい値
    static const float THRESHOLD = 0.1f;

    //デルタタイム取得
    float delta = SceneManager::GetInstance().GetDeltaTime();

    //補正時間カウンタ
    static float aimTime = _time;

    //しきい値判定
    if (aimTime > THRESHOLD)aimTime = _time;

    //デルタタイムで補間
    aimTime -= delta;

    //回転
    Quaternion rot = Quaternion::Identity();

    //回転を加える
    rot = rot.Mult(Quaternion::Euler(_toTargetAxis));

    //回転用に座標を相対座標から0,0,0にそろえる
    _trans.pos = VSub(_trans.pos, _trans.quaRot.PosAxis(_relativePos));

    //反映
    _trans.quaRot = Quaternion::Slerp(_trans.quaRot, rot, (_time - aimTime) / _time);

    //座標を元に戻す
    _trans.pos = VAdd(_trans.pos, _trans.quaRot.PosAxis(_relativePos));

    //基本情報更新
    _trans.Update();
}

bool Utility::IsPointInRect(const Vector2 _pos, const Vector2 _leftTop, const Vector2 _rightBotm)
{
    //指定の範囲内に座標があるか調べる
    return _pos.x > _leftTop.x && _pos.x < _rightBotm.x &&
        _pos.y > _leftTop.y && _pos.y < _rightBotm.y;
}

bool Utility::IsPointInRectCircle(const Vector2 _pos, const Vector2 _circlePos, const float _radius)
{
    //座標が円の範囲内か調べる(距離と円の半径を比べる)
    return Distance(_pos, _circlePos) <= _radius;
}

VECTOR Utility::GetWorldPosAtScreen(const Vector2 screenPos, const float distance, const VECTOR cameraPos, const VECTOR cameraDir)
{
    // スクリーン中心の方向ベクトルを取得 (depth = 0.5で中間点)
    VECTOR sPos = VGet(screenPos.x, screenPos.y, 0.5f);
    VECTOR screenDir = ConvScreenPosToWorldPos(sPos);

    // カメラ位置から見たスクリーン中心方向へのベクトルを作成
    VECTOR dir = VSub(screenDir, cameraPos);
    dir = VNorm(dir); // 正規化して単位ベクトルにする

    // 指定距離だけ進めた座標
    VECTOR ret = VAdd(cameraPos, VScale(dir, distance));
    return ret;
}

int Utility::GetSign(float f)
{
    return f == 0 ? 0 : f < 0 ? -1 : 1;
}

int Utility::GetSign(int f)
{
    return f == 0 ? 0 : f < 0 ? -1 : 1;
}

std::string Utility::OpenFileDialog()
{
    // --- 現在のカレントディレクトリを保存 ---
    DWORD buffer_size = GetCurrentDirectoryA(0, nullptr);
    std::string original_cwd;
    if (buffer_size != 0) {
        std::vector<char> current_dir_buffer(buffer_size);
        if (GetCurrentDirectoryA(buffer_size, current_dir_buffer.data()) != 0) {
            original_cwd = current_dir_buffer.data();
        }
    }
    // エラーハンドリングは必要に応じて追加してください。

    char filename[MAX_PATH] = "";

    OPENFILENAMEA ofn = {};
    ofn.lStructSize = sizeof(OPENFILENAMEA);
    ofn.lpstrFile = filename;
    ofn.nMaxFile = sizeof(filename);
    ofn.lpstrFilter = "JSON Files\0*.json\0All Files\0*.*\0";
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    std::string selected_path = ""; // 選択されたパスを保持する変数

    if (GetOpenFileNameA(&ofn)) // ファイルを開くダイアログを表示
    {
        selected_path = std::string(filename);
    }

    // --- 元のカレントディレクトリに戻す ---
    if (!original_cwd.empty()) {
        SetCurrentDirectoryA(original_cwd.c_str());
    }
    // エラーハンドリングは必要に応じて追加してください。

    return selected_path; // キャンセルされた場合やエラー時は空文字列を返す
}

VECTOR Utility::ReverseValue(VECTOR _vec)
{
    return VScale(_vec, REVERSE_SCALE);
}

int Utility::ReverseValue(int _i)
{
    return static_cast<int>(_i * REVERSE_SCALE);
}

float Utility::ReverseValue(float _f)
{
    return _f * REVERSE_SCALE;
}

IntVector3 Utility::ReverseValue(IntVector3 _iv)
{
    return _iv * REVERSE_SCALE;
}


int Utility::WrapIndex(int index, int max)
{
    return (index % max + max) % max;
}

void Utility::DrawPointLine3D(const VECTOR sPos, const VECTOR ePos, int color, float len)
{
	VECTOR dir = VNorm(VSub(ePos, sPos));
	VECTOR currentPos = sPos;
	while (true)
	{
		DrawLine3D(currentPos, VAdd(currentPos, VScale(dir, len)), color);
		currentPos = VAdd(currentPos, VScale(dir, len * 2.0f));
		if (Magnitude(VSub(currentPos, ePos)) < len)
		{
			break;
		}
	}
}

float Utility::EaseInQuad(const float _time, const float _totalTime, const float _start, const float _end)
{  
    float distance =  _end - _start;
    float t = _time / _totalTime;
    return distance * t * t + _start;
}

float Utility::EaseOutQuad(const float _time, const float _totalTime, const float _start, const float _end)
{
    float distance = _end - _start;
    float t = _time / _totalTime;
    return -distance * _time * (_time - 2) + _start;
}

float Utility::EaseInOutBack(float _time, const float _totalTime, const float _start, const float _end)
{
    // 補間定数1
    constexpr float C1 = 1.70158f;

    // 補間定数2（オーバーシュート調整用）
    constexpr float C2 = C1 * 1.525f;

    // 開始から終了までの距離
    float distance = _end - _start;

    // 補間係数
    float t = _time / _totalTime;

    // Clamp t to [0, 1]
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    // 補間の実行（EaseInOutBack）
    float easedT;
    if (t < 0.5f)
    {
        float twoT = 2.0f * t;
        easedT = (twoT * twoT * ((C2 + 1.0f) * twoT - C2)) / 2.0f;
    }
    else
    {
        float twoTMinus2 = 2.0f * t - 2.0f;
        easedT = (twoTMinus2 * twoTMinus2 * ((C2 + 1.0f) * twoTMinus2 + C2) + 2.0f) / 2.0f;
    }

    // 値の補間
    return distance * easedT + _start;
}

float Utility::GetShake(const float _value, float _time, const float _speed, const float _amplitude)
{
    return _value + std::sin(_time * _speed) * _amplitude;
}

int Utility::GetDigit(const int _value, const int _digit)
{
    //負の値にも対応するため絶対値を取る
    int ret = (_value < 0) ? -_value : _value;

    //10のdigit乗で割って1の位を抽出
    for (int i = 0; i < _digit; ++i)
    {
        ret /= 10;
    }

    return ret % 10;
}

int Utility::GetDigitCount(const int _value)
{
    // 0 の桁数は 1
    if (_value == 0) return 1;

    // マイナス値は絶対値に変換
    int ret = std::abs(_value);

    int digit = 0;
    while (ret > 0)
    {
        ret /= 10;
        ++digit;
    }

    return digit;
}

std::string Utility::ShowSaveJsonDialog()
{
    // --- 現在のカレントディレクトリを保存 ---
      // GetCurrentDirectoryA で必要なバッファサイズを取得
    DWORD buffer_size = GetCurrentDirectoryA(0, nullptr);
    std::string original_cwd;
    if (buffer_size != 0) {
        std::vector<char> current_dir_buffer(buffer_size);
        if (GetCurrentDirectoryA(buffer_size, current_dir_buffer.data()) != 0) {
            original_cwd = current_dir_buffer.data();
        }
    }
    // ここでエラーチェックを省略していますが、実際のアプリケーションではエラーハンドリングをしっかり行うことを推奨します。

    // 構造体をゼロ初期化
    OPENFILENAMEA ofn = {}; // Aサフィックスの構造体を使用

    // ファイル名バッファをマルチバイト文字 (char) に変更
    char fileName[MAX_PATH] = "";

    ofn.lStructSize = sizeof(OPENFILENAMEA);
    ofn.lpstrFilter = "JSONファイル (*.json)\0*.json\0すべてのファイル (*.*)\0*.*\0";
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_OVERWRITEPROMPT;
    ofn.lpstrDefExt = "json"; // 既定の拡張子もマルチバイト文字

    std::string selected_path = ""; // 選択されたパスを保持する変数

    if (GetSaveFileNameA(&ofn)) // Aサフィックスの関数を使用
    {
        // 取得したパスは、システムの現在のANSIコードページ (通常はSJIS) でエンコードされています。
        // そのまま std::string に変換して返します。
        selected_path = std::string(fileName);
    }

    // --- 元のカレントディレクトリに戻す ---
    if (!original_cwd.empty()) {
        SetCurrentDirectoryA(original_cwd.c_str());
    }
    // ここでもエラーチェックを省略していますが、SetCurrentDirectoryA が失敗する可能性も考慮してください。

    return selected_path; // キャンセルされたときには空文字列が返る
}

std::string Utility::WideToUtf8(const std::wstring& wstr)
{
    int size = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string result(size, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &result[0], size, nullptr, nullptr);
    result.pop_back(); // null文字を削除
    return result;
}

float Utility::PingPongUpdate(const float _value, const float _step, const float _max, const float _min, int& _dir)
{
    // 値を更新（directionは1または-1）
    float value = _value;
    value += _step * _dir;

    // 最大を超えたら減少に切り替え
    if (value >= _max)
    {
        value = _max;
        _dir = -1;
    }
    // 最小を下回ったら増加に切り替え
    else if (value <= _min)
    {
        value = _min;
        _dir = 1;
    }

    return value;
}
