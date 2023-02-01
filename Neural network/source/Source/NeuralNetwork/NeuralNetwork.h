/**
 * @file NeuralNetwork.h
 * @authors Gentzane Pastor Oleagoitia, Gaizka Calvo Orbe // gentzane.pastor, gaizka.c // 540000217, 540001517
 * @date 07/07/2021
 * @copyright Copyright (C) 2021 DigiPen Institute of Technology.
 * @brief Neural network class and function declarations.
 */

#pragma once

class Neuron
{
public:
	Neuron();
	~Neuron();

	void InitializeWeights(int connection_number_);

	float m_output;
	float m_input;

	float m_gradient;

	std::vector<float> m_next_layer_weights;
};

class NeuralNetwork
{
public:
	NeuralNetwork();

	void Initialize();

	void ForwardPropagation();
	void BackwardPropagation();

	float ActivationFunction(float x);

	void Test();
	void Test2();

	void PrintNodes();

	void CopyNetwork(const NeuralNetwork & rhs);


	int m_number_of_layers = 3;

	float m_learning_rate;

	std::vector<std::vector<Neuron>> m_neuron_layers;
	std::vector<float> m_real_outputs;

	int m_input_layer_size;
	int m_hiden_layer_size;
	int m_output_layer_size;
};

class ExperienceInfo
{
public:

	//states ID
	int m_state = 0;

	//action -> direction
	int m_action = 0;

	//Next state ID
	int m_next_state = 0;

	//Reward that we got
	float m_reward = 0;

	//if we need to use only the reward
	bool final = false;
};

class Q_Learning
{
public:

	void Initialize();

	void Update();

	void Learns();



	void CalculateState();

	NeuralNetwork m_network_target;
	NeuralNetwork m_network_main;
private:

	//states
	int m_apple_is_up    = 0;
	int m_apple_is_down  = 0;
	int m_apple_is_left  = 0;
	int m_apple_is_right = 0;

	int m_obstacle_up = 0;
	int m_obstacle_down   = 0;
	int m_obstacle_on_right = 0;
	int m_obstacle_on_left  = 0;

	int m_direction_right = 0;
	int m_direction_left  = 0;
	int m_direction_up    = 0;
	int m_direction_down  = 0;

	//number of replays that we will appy per step
	int m_max_batch  = 1000;
	int m_batch_tryes  = 500;
	//to calculate the Q value
	float m_future_reward_value = 0.95f;

	//for random exploration 
	float m_epsilon_min   = 0.001f;
	float m_epsilon_decay = 0.001f;

	//to update the target network
	int m_iterations_to_update_target = 250;
	int m_actual_iteration_numbers = 0;
	bool m_updated_once = false;

	//replay buffer
	std::vector<ExperienceInfo> m_reply_buffer;

	float m_died_reward;
	float m_got_apple_reward;
	float m_got_closer_from_apple_reward;
	float m_got_further_from_apple_reward;


public:

	//to calculate the reward 
	int m_row_distance = 0;
	int m_col_distance = 0;

	float m_epsilon_value = 0.99f;

};



class SnakeGame
{
public:
	SnakeGame();

	void Initialize();

	void Update();

	void Reset();

	void SetDirection(unsigned new_direction)
	{ m_direction = new_direction; }


	unsigned GetDirection()
	{ return m_direction; }

private:
	unsigned int  m_board_witdh;
	unsigned int  m_movements;
	unsigned int  m_direction;
	// 0 up 
	// 1 right
	// 2 down
	// 3 left
	bool m_increase_size;

public:
	float m_time_to_move;
	float m_timer;



	std::vector<std::pair<int, int>> m_snake; 

	unsigned int  m_moved_without_taking_apple;

	int m_apple_position_r;
	int m_apple_position_c;

	bool m_snake_died;
	bool m_snake_took_apple;

	unsigned int  m_max_apple_count;
	unsigned int  m_apple_count;
	unsigned int  m_total_apple_count;
	unsigned int  m_died_count;
};