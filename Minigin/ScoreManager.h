#include "TextComponent.h"
#include "EventManagers.h"
#include <sstream>

namespace dae
{
    class ScoreComponent final : public Observer 
	{
    public:
        void Update() override
        {
        }

        void Render() const override
        {
        }

		ScoreComponent(GameObject* pOwner) : Observer(pOwner), m_Score{ 0 }, m_Subject{ std::make_unique<Subject>() }
        {
        }

		virtual void OnNotify(const Event& event) override 
		{

			Event e{ EventType::ScoreChanged, this };
			switch (event.event)
			{
			case EventType::PlayerDead:
				m_Score--;
				m_Subject.get()->Notify(e);
				break;

			default:
				// do nothing 
				break;

			}
		}

		void SetScore(int score) 
		{
			m_Score = score;
			Event e{ EventType::ScoreChanged, this };
			m_Subject.get()->Notify(e);
			if (m_Score >= 500)
			{
				Event esteam{ EventType::Win, this };
				m_Subject.get()->Notify(esteam);

			}
		}

		void AddScore(int score)
		{
			m_Score += score;
			Event e{ EventType::ScoreChanged, this };
			m_Subject.get()->Notify(e);
			if (m_Score >= 500)
			{
				Event esteam{ EventType::Win, this };
				m_Subject.get()->Notify(esteam);
				
			}


		}


        Subject* GetSubject() { return m_Subject.get(); }
        int GetScore() { return m_Score; }

    private:
        int m_Score;
        std::unique_ptr<Subject> m_Subject;
    };


	class ScoreObserver : public Observer
	{
	public:
		ScoreObserver(GameObject* pOwner) : Observer(pOwner)
		{
		}

		~ScoreObserver() = default;

		virtual void OnNotify(const Event& event) override
		{
			std::stringstream ss;
			switch (event.event)
			{
			case EventType::ScoreChanged:
				ss << std::to_string(dynamic_cast<ScoreComponent*>(event.sender)->GetScore()) << " Score";
				if (auto textComp = m_pOwner->GetComponentByType<TextComponent>()) textComp->SetText(ss.str());
				break;

			default:
				// do nothing 
				break;
			}
		}

	};




}