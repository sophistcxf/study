/*!
 * implementation of https://en.wikipedia.org/wiki/Viterbi_algorithm
 */
#include "viterbi.h"

// 隐藏状态有 Healthy, Fever
// 观察状态有 normal, cold（感冒）, dizzy（头晕）
// 初始是 healthy 的概率是0.6，fever 的概率是0.4
// 转移函数 transition_prob_
// 观察概率 emission_prob_
void HMM::init(void) {
    states_.push_back("Healthy");
    states_.push_back("Fever");

    observations_.push_back("normal");
    observations_.push_back("cold");
    observations_.push_back("dizzy");

    initial_prob_["Healthy"] = 0.6;
    initial_prob_["Fever"] = 0.4;

    transition_prob_["Healthy"] = std::map<State, Probability>();
    transition_prob_["Healthy"]["Healthy"] = 0.7;
    transition_prob_["Healthy"]["Fever"] = 0.3;
    transition_prob_["Fever"] = std::map<State, Probability>();
    transition_prob_["Fever"]["Healthy"] = 0.4;
    transition_prob_["Fever"]["Fever"] = 0.6;

    emission_prob_["Healthy"] = std::map<State, Probability>();
    emission_prob_["Healthy"]["dizzy"] = 0.1;
    emission_prob_["Healthy"]["cold"] = 0.4;
    emission_prob_["Healthy"]["normal"] = 0.5;
    emission_prob_["Fever"] = std::map<State, Probability>();
    emission_prob_["Fever"]["dizzy"] = 0.6;
    emission_prob_["Fever"]["cold"] = 0.3;
    emission_prob_["Fever"]["normal"] = 0.1;
}

struct Tractor
{
    Tractor() : prob(1.0) {}
    StateSeq path;
    Probability prob;
};

/*!
 * @param obs 观察状态序列
 * @param states 隐藏状态表
 * @param init_p 初始概率
 * @param trans_p 转移函数表
 * @param emit_p 观察概率表
 */
void forward_viterbi(const ObservationSeq &obs, 
        const StateSpace& states, 
        const InitialProb& init_p, 
        const TransitionProb& trans_p, 
        const EmissionProb& emit_p)
{
    // tractors 存储到前一观察状态时，所有隐藏状态的概率最大的路径
    std::vector<Tractor> tractors;
    for (ObservationSeq::const_iterator it_obs = obs.begin(); it_obs != obs.end(); ++it_obs)
    {
        if (!tractors.empty())
        {
            std::vector<Tractor> temp_tractors;
            for (StateSpace::const_iterator it_s = states.begin(); it_s != states.end(); ++it_s)
            {
                temp_tractors.push_back(Tractor());
                // 寻找到隐藏状态 *it_s 概率最大的路径
                // 即到前一回合的隐藏状态的所有路径，剩上转移函数和观察概率
                Probability max_prob = -1.0;
                for (std::vector<Tractor>::iterator it_t = tractors.begin(); it_t != tractors.end(); ++it_t)
                {
                    Probability cur_prob = it_t->prob * trans_p.at(it_t->path.back()).at(*it_s) * emit_p.at(*it_s).at(*it_obs);
                    if (cur_prob  > max_prob)
                    {
                        max_prob = cur_prob;
                        temp_tractors.back() = *it_t;
                    }
                }
                temp_tractors.back().path.push_back(*it_s);
                temp_tractors.back().prob = max_prob;
            }
            // for 循环结束后，temp_tractors 中存储的是 states.size() 个元素，对应 states.size() 个隐藏状态
            // 记录了到每个隐藏状态的最大概率和路径
            tractors.swap(temp_tractors);
        }
        else
        {
            for (StateSpace::const_iterator it_s = states.begin(); it_s != states.end(); ++it_s)
            {
                tractors.push_back(Tractor());
                tractors.back().path.push_back(*it_s);
                // 观察到状态 *it_obs，其映射到 *it_s 的概率
                tractors.back().prob *= emit_p.at(*it_s).at(*it_obs) * init_p.at(*it_s);
            }
        }
    }
    int max_index = 0;
    for (size_t i = 1; i < tractors.size(); ++i)
    {
        if (tractors[i].prob > tractors[max_index].prob)
            max_index = i;
    }
    for (StateSeq::iterator it = tractors[max_index].path.begin();
         it != tractors[max_index].path.end(); ++it)
    {
        std::cout << *it << "\t";
    }
    std::cout << std::endl;
    std::cout << "max probability is " << tractors[max_index].prob << std::endl;
}
