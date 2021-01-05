#pragma once

namespace SDFix
{
	class DataHandler
	{

	public:

		static DataHandler* GetSingleton()
		{
			static DataHandler singleton;

			return &singleton;
		}


		bool AddHitAngleRecord(RE::FormID thisID, float headAngle)
		{
			if (HitAngleMap.size() > max_nums)
			{
				logger::info("Map Elements Number is reach to the Maximum!");
				return false;
			}

			if (HitAngleMap.count(thisID) > 0)
			{
				logger::info("Target Already in Map!");
				HitAngleMap.at(thisID) = headAngle;
			}
			else
				HitAngleMap.emplace(thisID, headAngle);

			return true;
		}


		void RemoveHitAngleRecord(const RE::FormID& thisID)
		{
			HitAngleMap.erase(thisID);
		}


		bool GetHitAngle(const RE::FormID &thisID, float &headAngle) const
		{
			if (HitAngleMap.count(thisID) > 0)
			{
				headAngle = HitAngleMap.at(thisID);
				return true;
			}

			return false;
		}

		bool HasHitAngleRecord(const RE::FormID& thisID)
		{
			return HitAngleMap.count(thisID) > 0;
		}


		inline size_t GetMapSize()
		{
			return HitAngleMap.size();
		}

		void ClearMap()
		{
			HitAngleMap.clear();
		}






	private:

		DataHandler() = default;

		~DataHandler() = default;

		DataHandler(const DataHandler&) = delete;

		DataHandler(DataHandler&&) = delete;

		DataHandler& operator= (const DataHandler&) = delete;

		DataHandler& operator= (DataHandler&&) = delete;



		std::unordered_map <RE::FormID, float>   HitAngleMap;

		const std::uint32_t						 max_nums = 500;

	};





}