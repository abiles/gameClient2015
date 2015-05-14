
class Timer
{
public:
	Timer();
	~Timer();

	float deltaTime() const;
	float totalTime() const;

	void reset();
	void start();
	void stop();
	void tick(); // �������Ӹ��� ȣ���

private:
	double m_SecondPerCount = 0.0;
	double m_DeltaTime = -1.0;

	__int64 m_BaseTime = 0;
	__int64 m_PauseTime = 0;
	__int64 m_StopTime = 0;
	__int64 m_PrevTime = 0;
	__int64 m_CurTime = 0;

	bool m_Stopped = false;
};
