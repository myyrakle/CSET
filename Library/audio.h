#define SFML_STATIC
#include <SFML/Audio.hpp>

class MusicPlayer
{
private:
	sf::Music music;
	bool opened = false;
public:
	operator bool() const
	{
		return opened;
	}
	bool operator!() const
	{
		return !opened;
	}
public:
	bool open_file(const String& filename)
	{
		try 
		{
			std::string temp;

			for (const auto& e : filename)
				temp += e;

			music.openFromFile(temp);
			opened = true;
			return opened;
		}
		catch (...)
		{
			return !opened;
		}
	}
	bool is_opened() const
	{
		return opened;
	}
public:
	int get_music_length() const
	{
		return music.getDuration().asSeconds();
	}
public:
	void set_playing_offset(long long second)
	{
		music.setPlayingOffset(sf::seconds(second));
	}
	long long get_playing_offset() const
	{
		return (music.getPlayingOffset().asSeconds());
	}
public:
	void set_volume(float f) //볼륨을 설정합니다. 최대 100
	{
		music.setVolume(f);
	}
	float get_volume() const //현재 볼륨을 가져옵니다.
	{
		return music.getVolume();
	}
	void set_pitch(float f) 
	{
		music.setPitch(f);
	}
	float get_pitch() const
	{
		return music.getPitch();
	}
	bool is_auto_repeatable() const //자동으로 반복하는지 확인합니다.
	{
		return music.getLoop();
	}
	void set_auto_repeat(bool _do) //자동반복여부를 설정합니다.
	{
		music.setLoop(_do);
	}
public:
	void play() //비동기로 재생합니다.
	{
		music.play();
	}
	void play_with_block(void(*func)(void) = nullptr) //동기로 재생합니다.
	{
		music.play();
		block_until_ended(func);
	}
	void block_until_ended(void(* func)(void) = nullptr) //재생하고, 끝날때까지 락을 겁니다. 콘솔 전용
	{
		while (this->is_playing())
			if (func != nullptr)
				func();
	}
	void pause() //정지합니다.
	{
		music.pause();
	}
	void stop() //중단합니다.
	{
		music.stop();
	}
public:
	bool is_playing() const //재생여부를 확인합니다.
	{
		return music.getStatus() == sf::Music::Status::Playing;
	}
	bool is_paused() const //정지여부를 확인합니다.
	{
		return music.getStatus() == sf::Music::Status::Paused;
	}
	bool is_stopped() const //중단여부를 확인합니다.
	{
		return music.getStatus() == sf::Music::Status::Stopped;
	}
};