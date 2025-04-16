class Framework
{
public:
	Framework();
	virtual ~Framework() = default;

	virtual void Initialize();

	virtual void Finalize();

	virtual void Update();

	virtual void Draw() = 0;

	virtual bool IsEndRequest() { return endRequest_; }

	void Run();

protected:
	bool endRequest_ = false;
};
