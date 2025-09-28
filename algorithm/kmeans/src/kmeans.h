#include <vector>
#include <limits>
#include <stdlib.h>

/**
 * 模板参数:
 *		Sample 为样本类
 *		CoorsGetor 为从Sample 中得到坐标
 */
template<typename Sample, typename CoorsGetor>
class KMeans
{
public:
	typedef std::vector<double> SampleCoor;
	bool cluster(const std::vector<Sample*>& samples, unsigned cluster_size, unsigned max_iter = 300)
	{
		if (samples.empty() || cluster_size == 0 || max_iter == 0 || cluster_size >= samples.size())
		{
			return false;
		}
		std::vector<SampleCoor> sample_coors;
		inertia_ = std::numeric_limits<double>::max();
		sample_coors.resize(samples.size());
		for (unsigned i = 0; i < sample_coors.size(); ++i)
		{
			sample_coors[i] = getor_(samples[i]);
		}
		unsigned dimensions = sample_coors[0].size();
		cluster_impl(sample_coors, cluster_size, dimensions, max_iter);
		return true;
	}

	std::vector<unsigned> labels()
	{
		return labels_;
	}

	float inertia()
	{
		return inertia_;
	}

	std::vector<std::vector<double> > centers()
	{
		return centers_;
	}

private:
	void cluster_impl(const std::vector<SampleCoor>& samples, unsigned cluster_size, unsigned dimensions, unsigned max_iter)
	{
		// 初始化聚类中心
		std::vector<unsigned> v(samples.size(), 0);
		for (unsigned i = 0; i < samples.size(); ++i)
			v[i] = i;

		int n = v.size();
		std::vector<unsigned> init_idx(cluster_size, 0);
		for (unsigned i = 0; i < cluster_size; ++i)
		{
			int idx = rand() % n;
			init_idx[i] = v[idx];	
			v[idx] = v[n-1];
			n--;
		}

		std::vector<std::vector<double> > centers;	
		centers.resize(cluster_size);
		for (unsigned i = 0; i < cluster_size; ++i)
		{
			centers[i] = samples[init_idx[i]];
		}
		for (unsigned i = 0; i < max_iter; ++i)
		{
			std::vector<unsigned> labels;
			labels.resize(samples.size(), 0);
			float inertia = single_round(samples, centers, dimensions, labels);
			if (inertia >= inertia_)
			{
				break;
			}
			inertia_ = inertia;
			centers_ = centers;
			labels_ = labels;
		}
	}

	float single_round(const std::vector<SampleCoor>& samples, std::vector<SampleCoor>& centers, unsigned dimensions, std::vector<unsigned>& labels)
	{
		std::vector<SampleCoor> new_centers;
		new_centers.resize(centers.size(), SampleCoor(dimensions, 0));
		std::vector<unsigned> cluster_size(centers.size(), 0);
		float inertia = 0.0;
		for (unsigned i = 0; i < samples.size(); ++i)
		{
			float min_dist = std::numeric_limits<float>::max();
			for (unsigned j = 0; j < centers.size(); ++j)
			{
				float dist = distance(samples[i], centers[j]);
				if (dist < min_dist)
				{
					labels[i] = j;
					min_dist = dist;
				}
			}
			for (unsigned j = 0; j < dimensions; ++j)
			{
				new_centers[labels[i]][j] += samples[i][j];
			}
			cluster_size[labels[i]]++;
			inertia += min_dist;
		}
		// 重算聚类中心
		for (unsigned i = 0; i < centers.size(); ++i)
		{
			for (unsigned j = 0; j < dimensions; ++j)
			{
				centers[i][j] = new_centers[i][j] * 1.0 / cluster_size[i];
			}
		}
		return inertia;
	}

	float distance(const SampleCoor& c1, const SampleCoor& c2)
	{
		float sum = 0.0;
		for (unsigned i = 0; i < c1.size(); ++i)
		{
			sum += (c1[i] - c2[i]) * (c1[i] - c2[i]);
		}
		return sum;
	}

private:
	std::vector<unsigned> labels_;
	std::vector<std::vector<double> > centers_;
	float inertia_;
	CoorsGetor getor_;
};
