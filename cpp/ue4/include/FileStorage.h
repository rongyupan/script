#ifndef _FILE_STORAGE_H
#define _FILE_STORAGE_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <memory>
#include <assert.h>

namespace vision215
{
	enum Mode
	{
		r = 0,
		w,
		app
	};

	class ConfigFileStorage
	{
	private:
		static std::shared_ptr<ConfigFileStorage> config_;
		cv::FileStorage file_;
		Mode mode_;

		ConfigFileStorage() {};

	public:
		~ConfigFileStorage();

		static void setParameterFile(const std::string &filename, Mode mode);

		//get
		template<typename T>
		static T get(const std::string &key) {
			if (ConfigFileStorage::config_->mode_ != 0)
			{
				std::cerr << "Mode Error!!!" << std::endl;
				exit(0);
			}
			T result;
			ConfigFileStorage::config_->file_[key] >> result;
			return result;
		}

		//set
		template<typename T>
		static void set(const std::string &key, const T &value)
		{
			if (ConfigFileStorage::config_->mode_ == Mode::r)
			{
				std::cerr << "Mode Error!!!" << std::endl;
				return;
			}
			ConfigFileStorage::config_->file_ << key << value;
		}
	};
}

#endif // !_FILE_STORAGE_H
