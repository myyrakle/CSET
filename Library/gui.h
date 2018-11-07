#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <string>
#include <functional>

using Event = sf::Event::EventType;
using Keyboard = sf::Keyboard;
using Color = sf::Color;
using Font = sf::Font;

struct point
{
	int x;
	int y;
};

struct quad
{
	unsigned int width;
	unsigned int height;
};

class EventInfo
{
private:
	const sf::Event& event;
public:
	EventInfo(const sf::Event& event) : event(event)
	{}
public:
	EventInfo() = delete;
	virtual ~EventInfo() = default;
	EventInfo(const EventInfo&) = delete;
	EventInfo& operator=(const EventInfo&) = delete;
	EventInfo(EventInfo&&) = default;
	EventInfo& operator=(EventInfo&&) = default;
public:
	unsigned int getchar() const //눌린 문자 반환
	{
		return event.text.unicode;
	}
	point get_moved_mouse() const //현재 마우스위치 반환
	{
		return point{ event.mouseMove.x, event.mouseMove.y };
	}
	quad get_resized_size() const //현재 창 크기 반환
	{
		return quad{ event.size.width, event.size.height };
	}
	sf::Keyboard::Key get_key() const //눌린 키
	{
		return event.key.code;
	}
public:
	/*
	mouse_pressed/mose_released 관련 메서드
	*/
	bool is_left_button() const //왼쪽 마우스 버튼이 눌렸다면 true
	{
		return event.mouseButton.button == sf::Mouse::Button::Left;
	}
	bool is_right_button() const //오른쪽 마우스 버튼이 눌렸다면 true
	{
		return event.mouseButton.button == sf::Mouse::Button::Right;
	}
	point get_clicked_position() const //마우스 버튼으로 클릭한 위치 반환
	{
		return point{ event.mouseButton.x, event.mouseButton.y };
	}
	point get_scrolled_position() const
	{
		return point{ event.mouseWheelScroll.x, event.mouseWheelScroll.y };
	}
	float get_scrolled_ticks() const
	{
		return event.mouseWheelScroll.delta;
	}
};

class Window
{
private:
	sf::RenderWindow window;
	sf::Event event;
	sf::String title = L"이름 없음";
	quad window_size = { 200,200 };
	int window_style = sf::Style::Default;
	bool created = false;
public:
	Window() : window()
	{
		on_started = [](Window& window)
		{
			window.cover_with_color(sf::Color::White);
			window.display();
		};

		on_resized = [](Window& window, quad sizes)
		{
			window.display();
		};

		on_closed = [](Window& window)
		{
			window.close();
		};
	}
	virtual ~Window() = default;
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	Window(Window&&) = default;
	Window& operator=(Window&&) = default;
public:
	void run()
	{
		if (created == false)
			this->create();

		if (on_started != nullptr)
			on_started(*this);

		while (this->is_open())
		{
			if (on_always != nullptr) //항상 실행
				on_always(*this);

			while (this->poll_event())
			{
				auto current_event = this->get_event(); //현재 이벤트
				auto info = this->get_event_info(); //이벤트 추가정보

				if (pre_translated_events != nullptr)
					pre_translated_events(*this, current_event);

				if (this->before_handling_events != nullptr)
					before_handling_events(*this);

				switch (current_event) //이벤트 분기
				{
				case sf::Event::EventType::Closed: //닫힐 때 수행됩니다.
					if (on_closed != nullptr)
						on_closed(*this);
					break;

				case sf::Event::EventType::TextEntered: //문자 입력시 수행됩니다.
					if (on_text_entered != nullptr)
						on_text_entered(*this, info.getchar());
					break;

				case sf::Event::EventType::KeyPressed: //키가 눌렸을 때 수행됩니다.
					if (on_key_down)
						on_key_down(*this, info.get_key());
					break;

				case sf::Event::EventType::KeyReleased: //키가 떼였을 때 수행됩니다.
					if (on_key_up)
						on_key_up(*this, info.get_key());
					break;

				case sf::Event::EventType::MouseEntered: //마우스가 화면에 진입하면 수행됩니다.
					if (on_mouse_entered != nullptr)
						on_mouse_entered(*this);
					break;

				case sf::Event::EventType::MouseLeft: //마우스가 화면을 벗어나면 수행됩니다.
					if (on_mouse_left != nullptr)
						on_mouse_left(*this);
					break;

				case sf::Event::EventType::MouseWheelScrolled: //휠이 돌아갈 때 수행됩니다.
					if (on_wheel_scrolled != nullptr)
						on_wheel_scrolled(*this, info.get_scrolled_position().x, info.get_scrolled_position().y,
							info.get_scrolled_ticks());
					break;

				case sf::Event::EventType::MouseMoved: //마우스가 움직일 때 수행됩니다.
					if (on_mouse_move != nullptr)
						on_mouse_move(*this, { info.get_moved_mouse().x, info.get_moved_mouse().y });
					break;

				case sf::Event::EventType::MouseButtonPressed: //버튼이 눌릴 때 수행됩니다.
				{
					if (info.is_left_button()) //왼쪽 버튼이 눌릴 때
					{
						auto pair = info.get_clicked_position();
						if (on_left_button_down != nullptr)
							on_left_button_down(*this, { pair.x, pair.y });
					}
					else if (info.is_right_button()) //오른쪽 버튼이 눌릴 때
					{
						auto pair = info.get_clicked_position();
						if (on_right_button_down != nullptr)
							on_left_button_down(*this, { pair.x, pair.y });
					}
					else
					{
						//???
					}
					break;
				}

				case sf::Event::EventType::MouseButtonReleased: //버튼이 떼어질 때 호출됩니다.
				{
					if (info.is_left_button()) //왼쪽 버튼이 떼일 때
					{
						auto pair = info.get_clicked_position();
						if (on_left_button_up != nullptr)
							on_left_button_up(*this, { pair.x, pair.y });
					}
					else if (info.is_right_button()) //오른쪽 버튼이 떼일 때
					{
						auto pair = info.get_clicked_position();
						if (on_right_button_up != nullptr)
							on_left_button_up(*this, { pair.x, pair.y });
					}
					else
					{
						//???
					}
					break;
				}

				case sf::Event::EventType::GainedFocus: //포커스를 얻을 때 호출됩니다,
					if (on_gained_focus != nullptr)
						on_gained_focus(*this);
					break;

				case sf::Event::EventType::LostFocus: //포커스를 잃을 때 호출됩니다.
					if (on_lost_focus != nullptr)
						on_lost_focus(*this);
					break;

				case sf::Event::EventType::Resized: //크기가 변경될 때 호출됩니다.
					if (on_resized != nullptr)
						on_resized(*this, { info.get_resized_size().width, info.get_resized_size().height });
					break;

				default: //이외 작업에서 호출됩니다.
					if (on_default != nullptr)
						on_default(*this);
					break;
				}

				if (this->after_handling_events != nullptr)
					after_handling_events(*this);

				continue;
			}

			/*if (on_no_event != nullptr)
			on_no_event(*this);*/ //미구현
		}
	}
public: //이벤트 처리기입니다.
	std::function<void(Window&)> on_started = nullptr; //시작될때 한번만 호출됩니다.
	std::function<void(Window&)> on_always = nullptr; //항상 호출됩니다.
	std::function<void(Window&)> on_paint = nullptr;  //그리기 작업을 여기다 모아놓고 합니다.
	std::function<void(Window&)> on_no_event = nullptr; //아무런 이벤트도 없을 때 발생합니다. //아직 구현되지 않았습니다.

	std::function<void(Window&, sf::Event::EventType&)> pre_translated_events = nullptr; //감지된 이벤트에 추가적인 밑작업을 합니다.
	std::function<void(Window&)> before_handling_events = nullptr; //이벤트를 감지했을 때, 이벤트들을 처리하기 전에 호출됩니다.
	std::function<void(Window&)> after_handling_events = nullptr; //이벤트를 감지했을 때, 이벤트들을 처리한 후에 호출됩니다.

	std::function<void(Window&, unsigned int)> on_text_entered = nullptr; //문자 키가 눌렸을 때 호출됩니다.
	std::function<void(Window&, sf::Keyboard::Key)> on_key_down = nullptr; //키보드가 눌릴 때 호출됩니다.
	std::function<void(Window&, sf::Keyboard::Key)> on_key_up = nullptr; //키보드가 떼일 때 호출됩니다.	

	std::function<void(Window&)> on_gained_focus = nullptr; //포커스를 얻었을 때 호출됩니다.
	std::function<void(Window&)> on_lost_focus = nullptr; //포커스를 잃을 때 호출됩니다.

	std::function<void(Window&, point)> on_left_button_down = nullptr; //왼쪽 버튼을 누를 때 호출됩니다.
	std::function<void(Window&, point)> on_left_button_up = nullptr; //왼쪽 버튼을 뗄 때 호출됩니다.
	std::function<void(Window&, point)> on_right_button_down = nullptr; //오른쪽 버튼을 누를 때 호출됩니다.
	std::function<void(Window&, point)> on_right_button_up = nullptr; //오른쪽 버튼을 뗄 때 호출됩니다.

	std::function<void(Window&, point)> on_mouse_move = nullptr; //마우스가 움직일 때 호출됩니다.
	std::function<void(Window&)> on_mouse_entered = nullptr; //마우스가 창에 진입했을 때 호출됩니다.
	std::function<void(Window&)> on_mouse_left = nullptr; //마우스가 창에서 나갔을 때 호출됩니다.

	std::function<void(Window&, int, int, float)> on_wheel_scrolled = nullptr; //마우스 휠이 움직일 때 호출됩니다.

	std::function<void(Window&)> on_closed = nullptr; //닫힐 때 호출됩니다.
	std::function<void(Window&, quad)> on_resized = nullptr; //창 크기가 바뀔 때 호출됩니다.
	std::function<void(Window&)> on_default = nullptr; //기본 동작입니다.
public:
	void paint()
	{
		if (on_paint != nullptr)
			on_paint(*this);
	}
public:
	sf::Event::EventType get_event() const //현재 이벤트 반환
	{
		return event.type;
	}
	bool poll_event() //이벤트 확인
	{
		return window.pollEvent(this->event);
	}
public:
	bool is_open() const
	{
		return window.isOpen();
	}
	void close()
	{
		window.close();
	}
public:
	bool has_focus() const
	{
		return window.hasFocus();
	}
	void set_visible(bool want = true)
	{
		return window.setVisible(want);
	}
	void set_position(int x, int y)
	{
		window.setPosition(sf::Vector2i(x, y));
	}
public:
	void create()
	{
		this->create(window_size.width, window_size.height, title, window_style); //위임
	}
	void create(unsigned int _width, unsigned int _height, std::wstring _title, int _style) //생성자 함수
	{
		this->created = true;
		window.create(sf::VideoMode(_width, _height), _title, _style);
	}
public:
	void resizable(bool want) //true일 경우 윈도우의 크기를 조정할 수 있습니다.
	{
		if (want)
			window_style |= sf::Style::Resize;
		else
			window_style &= ~sf::Style::Resize;
	}
	void use_titlebar(bool want) //true일 경우 타이틀바가 표시됩니다.
	{
		if (want)
			window_style |= sf::Style::Titlebar;
		else
			window_style &= ~sf::Style::Titlebar;
	}
	void use_close_button(bool want) //true일 경우 닫기 버튼이 표시됩니다.
	{
		if (want)
			window_style |= sf::Style::Close;
		else
			window_style &= ~sf::Style::Close;
	}
	void use_default_window()
	{
		window_style = sf::Style::Default;
	}
	void use_fullscreen()
	{
		window_style = sf::Style::Fullscreen;
	}
	void use_none_decoration()
	{
		window_style = sf::Style::None;
	}
public: //창 크기 조절
	void set_size(unsigned int width, unsigned int height)
	{
		window_size = { width, height };
		window.setSize(sf::Vector2u(width, height));
	}
	void set_size(const quad& size)
	{
		set_size(size.width, size.height);
	}
	quad get_size() const
	{
		return quad{ window.getSize().x, window.getSize().y };
	}
	unsigned int get_width() const
	{
		return window_size.width;
	}
	unsigned int get_height() const
	{
		return window_size.height;
	}
public:
	void set_title(std::wstring title)
	{
		this->title = title;
		window.setTitle(title);
	}
	const sf::String& get_title() const
	{
		return this->title;
	}

	/*그래픽 처리*/
public:
	void cover_with_color(sf::Color color = sf::Color::White)
	{
		window.clear(color);
	}
	void add_drawable(sf::Drawable& drawable)
	{
		window.draw(drawable);
	}
	template<class SF_Drawable>
	void add_drawables(std::vector<SF_Drawable>& drawables)
	{
		for (auto& e : drawables)
			window.draw(e);
	}
	void display() //띄우기
	{
		window.display();
	}
public:
	static bool is_key_pressed(sf::Keyboard::Key key)
	{
		return sf::Keyboard::isKeyPressed(key);
	}
	static bool is_left_button_down()
	{
		return sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
	}
	static bool is_right_button_down()
	{
		return sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
	}
	static point get_mouse_position() //현재 마우스 좌표 반환
	{
		auto pos = sf::Mouse::getPosition();
		return point{ pos.x, pos.y };
	}
public:
	EventInfo get_event_info() const //이벤트 정보 처리 위임
	{
		return EventInfo(this->event);
	}
};


class ThinLine
{
private:
	sf::VertexArray line = sf::VertexArray(sf::LinesStrip, 2);
public:
	ThinLine() = default;
	virtual ~ThinLine() = default;
	void set_color(sf::Color color)
	{
		line[0].color = color;
		line[1].color = color;
	}
	void set_from_and_to(point from, point to)
	{
		line[0].position = { (float)from.x, (float)from.y };
		line[1].position = { (float)to.x, (float)to.y };
	}
	operator sf::VertexArray&()
	{
		return line;
	}
};
