/**
 * @file NeuralNetwork.cpp
 * @authors Gentzane Pastor Oleagoitia, Gaizka Calvo Orbe // gentzane.pastor, gaizka.c // 540000217, 540001517
 * @date 07/07/2021
 * @copyright Copyright (C) 2021 DigiPen Institute of Technology.
 * @brief Neural network class and function definitions.
 */

#include "Stdafx.h"

Neuron::Neuron(): m_output(0.0f), m_input(0.0f), m_gradient(0.0f), m_next_layer_weights()
{

}

Neuron::~Neuron()
{
}

void Neuron::InitializeWeights(int connection_number_)
{
	//Resize the layer weights
	m_next_layer_weights.resize(connection_number_);


	//Go through the weights
	for (int i = 0; i < connection_number_; i++)
	{
		//Intialize the weights to random values between -1 and 1
		m_next_layer_weights[i] = g_random.RangeFloat(-1.0f, 1.0f);
	}
}

NeuralNetwork::NeuralNetwork() : m_number_of_layers(3), m_learning_rate(0.1f), m_input_layer_size(0), m_hiden_layer_size(0), m_output_layer_size(0)
{

}

void NeuralNetwork::Initialize()
{

	//Resize the neural network layer
	m_neuron_layers.resize(m_number_of_layers);


	//Input layer resize
	m_neuron_layers[0].resize(m_input_layer_size + 1);

	//Go through the neural network layer
	for (int i = 1; i < m_number_of_layers - 1; i++)
	{
		//Hidden layer resize
		m_neuron_layers[i].resize(m_hiden_layer_size + 1);
	}
		
	//Output layer resize
	m_neuron_layers[m_number_of_layers - 1].resize(m_output_layer_size);

	//Go through the neural network layer
	for (int i = 0; i < m_number_of_layers-1; i++)
	{
		//Save the size of the current layer
		int size = m_neuron_layers[i].size();

		//Go thorugh the neurons of the layers
		for (int j = 0; j < size; j++)
		{
			//change the output of the bias to 1.0f
			if (j == size - 1)
				m_neuron_layers[i][j].m_output = 1.0f;

			//Check if the i value is the layers1-
			if (i == m_number_of_layers - 2)
			{
				//Set the value of the neurons
				m_neuron_layers[i][j].InitializeWeights(m_neuron_layers[i + 1].size());
			}
			
			//Other cases
			else
			{
				//Set the value of the neurons
				m_neuron_layers[i][j].InitializeWeights(m_neuron_layers[i + 1].size() - 1);
			}
		}
	}
}

void NeuralNetwork::ForwardPropagation()
{
	//Go through the neural network layer
	for (int i = 1; i < m_number_of_layers; i++)
	{
		int size = 0;
		if(i == m_number_of_layers - 1)
		//Save the size of the current layer
			size = m_neuron_layers[i].size();
		else
			size = m_neuron_layers[i].size() -1;


		//Go thorugh the neurons of the layers
		for (int j = 0; j < size; j++)
		{
			//Save the size of the previous layer
			int size2 = m_neuron_layers[i-1].size();

			//Reset the input value
			m_neuron_layers[i][j].m_input = 0.0f;

			//Go thorugh the neurons of the layers
			for (int k = 0; k < size2; k++)
			{
				//Compute the neuron weighted sum
				m_neuron_layers[i][j].m_input += m_neuron_layers[i-1][k].m_next_layer_weights[j] * m_neuron_layers[i - 1][k].m_output;
			}

			//Compute the output value
			m_neuron_layers[i][j].m_output = ActivationFunction(m_neuron_layers[i][j].m_input);
		}
	}
}

void NeuralNetwork::BackwardPropagation()
{
	//we start from the penultimate layer

	int index = m_neuron_layers.size() - 2;

	int size = m_neuron_layers[index].size();
	
	for (int i = 0; i < size; i++)
	{
		float d_e, d_o, d_net;

		//reset the gradient that we will use for future layers
		m_neuron_layers[index][i].m_gradient = 0.0f;

		//current output
		float current_output = m_neuron_layers[index][i].m_output;

		int outputs = m_neuron_layers[index][i].m_next_layer_weights.size();
		for (int j = 0; j < outputs; j++)
		{
			//output of the neural network
			float next_output = m_neuron_layers[index + 1][j].m_output;

			d_e = (next_output - m_real_outputs[j]);

			d_o = next_output * (1 - next_output);

			d_net = current_output;

			m_neuron_layers[index][i].m_gradient += d_e * d_o * m_neuron_layers[index][i].m_next_layer_weights[j];

			m_neuron_layers[index][i].m_next_layer_weights[j] = m_neuron_layers[index][i].m_next_layer_weights[j] - m_learning_rate * (d_e * d_o * d_net);
		}
	}


	//the rest of the layers
	for (int layer = m_neuron_layers.size() - 3; layer >= 0; layer--)
	{
		float d_e, d_o, d_net;


		int neurons = m_neuron_layers[layer].size();
		for (int i = 0; i < neurons; i++)
		{
			int connections = m_neuron_layers[layer][i].m_next_layer_weights.size();

			//reset the gradient that we will use for future layers
			m_neuron_layers[layer][i].m_gradient = 0.0f;

			//current output
			float current_output = m_neuron_layers[layer][i].m_output;

			for (int j = 0; j < connections; j++)
			{
				//output of the neural network
				float next_output = m_neuron_layers[layer + 1][j].m_output;

				d_e = m_neuron_layers[layer + 1][j].m_gradient;

				d_o = next_output * (1 - next_output);

				d_net = current_output;

				m_neuron_layers[layer][i].m_gradient += d_e * d_o * m_neuron_layers[layer][i].m_next_layer_weights[j];

				m_neuron_layers[layer][i].m_next_layer_weights[j] = m_neuron_layers[layer][i].m_next_layer_weights[j] - m_learning_rate * (d_e * d_o * d_net);
			}
		}
	}
}

float NeuralNetwork::ActivationFunction(float x)
{
	//Compute and return sigmoid function for the activation
	return 1 / (1 + expf(-x));
}

//Some test made to check the backward and forward propagation with the examples in the slides
void NeuralNetwork::Test()
{
	m_input_layer_size  = 1;
	m_hiden_layer_size  = 2;
	m_output_layer_size = 1;
	m_learning_rate = 0.1f;

	Initialize();

	m_neuron_layers[0][0].m_output = 5;
	m_neuron_layers[0][0].m_next_layer_weights[0] = 0.5f;
	m_neuron_layers[0][0].m_next_layer_weights[1] = 0.2f;

	m_neuron_layers[0][1].m_next_layer_weights[0] = 0.9f;
	m_neuron_layers[0][1].m_next_layer_weights[1] = -0.2f;

	m_neuron_layers[1][0].m_next_layer_weights[0] = -0.8f;

	m_neuron_layers[1][1].m_next_layer_weights[0] = 0.6f;

	m_neuron_layers[1][2].m_next_layer_weights[0] = 0.2f;


	PrintNodes();

	std::cout << "Forward propragation "  << std::endl;

	ForwardPropagation();

	PrintNodes();

	std::cout << "Backward propragation " << std::endl;

	m_real_outputs.push_back(0.6f);

	BackwardPropagation();

	PrintNodes();

}

//Some test made to check the backward and forward propagation with the examples in the slides
void NeuralNetwork::Test2()
{
	m_input_layer_size  = 2;
	m_hiden_layer_size  = 2;
	m_output_layer_size = 2;
	m_learning_rate = 0.5f;

	Initialize();

	m_neuron_layers[0][0].m_output = 0.05f;
	m_neuron_layers[0][0].m_next_layer_weights[0] = 0.15f;
	m_neuron_layers[0][0].m_next_layer_weights[1] = 0.25f;

	m_neuron_layers[0][1].m_output = 0.1f;
	m_neuron_layers[0][1].m_next_layer_weights[0] = 0.2f;
	m_neuron_layers[0][1].m_next_layer_weights[1] = 0.3f;


	m_neuron_layers[0][2].m_next_layer_weights[0] = 0.35f;
	m_neuron_layers[0][2].m_next_layer_weights[1] = 0.35f;

	m_neuron_layers[1][0].m_next_layer_weights[0] = 0.4f;
	m_neuron_layers[1][0].m_next_layer_weights[1] = 0.5f;

	m_neuron_layers[1][1].m_next_layer_weights[0] = 0.45f;
	m_neuron_layers[1][1].m_next_layer_weights[1] = 0.55f;

	m_neuron_layers[1][2].m_next_layer_weights[0] = 0.6f;
	m_neuron_layers[1][2].m_next_layer_weights[1] = 0.6f;


	PrintNodes();

	std::cout << "Forward propragation " << std::endl;

	ForwardPropagation();

	PrintNodes();

	std::cout << "Backward propragation " << std::endl;

	m_real_outputs.push_back(0.01f);
	m_real_outputs.push_back(0.99f);

	BackwardPropagation();

	PrintNodes();
}

//Debug print to check the node data 
void NeuralNetwork::PrintNodes()
{

	//Input  layer print
	for (unsigned i = 0; i < m_neuron_layers[0].size(); i++)
	{
		std::cout << "Input Layer input    #" << i <<": " << m_neuron_layers[0][i].m_input << std::endl;
		std::cout << "Input Layer output   #" << i <<": " << m_neuron_layers[0][i].m_output << std::endl;
		std::cout << "Input Layer gradient #" << i <<": " << m_neuron_layers[0][i].m_gradient << std::endl;

		for(unsigned j = 0; j < m_neuron_layers[0][i].m_next_layer_weights.size(); j++)
			std::cout << "Input Layer m_weights #" << i << ": " << m_neuron_layers[0][i].m_next_layer_weights[j] << std::endl;
	}
	std::cout<<std::endl;

	//Hidden  layer print
	for (unsigned i = 0; i < m_neuron_layers[1].size(); i++)
	{
		std::cout << "Hidden Layer input    #" << i <<": " << m_neuron_layers[1][i].m_input << std::endl;
		std::cout << "Hidden Layer output   #" << i <<": " << m_neuron_layers[1][i].m_output << std::endl;
		std::cout << "Hidden Layer gradient #" << i <<": " << m_neuron_layers[1][i].m_gradient << std::endl;

		for (unsigned j = 0; j < m_neuron_layers[1][i].m_next_layer_weights.size(); j++)
			std::cout << "Hidden Layer m_weights #" << i << ": " << m_neuron_layers[1][i].m_next_layer_weights[j] << std::endl;
	}
	std::cout << std::endl;

	//Output  layer print
	for (unsigned i = 0; i < m_neuron_layers[2].size(); i++)
	{
		std::cout << "Output Layer input    #" << i <<": " << m_neuron_layers[2][i].m_input << std::endl;
		std::cout << "Output Layer output   #" << i <<": " << m_neuron_layers[2][i].m_output << std::endl;
		std::cout << "Output Layer gradient #" << i <<": " << m_neuron_layers[2][i].m_gradient << std::endl;

		for (unsigned j = 0; j < m_neuron_layers[2][i].m_next_layer_weights.size(); j++)
			std::cout << "Output Layer m_weights #" << i << ": " << m_neuron_layers[2][i].m_next_layer_weights[j] << std::endl;
	}
	std::cout << std::endl;

}


//we expect them to have the same topology
void NeuralNetwork::CopyNetwork(const NeuralNetwork & rhs)
{
	for (int i = 0; i < rhs.m_number_of_layers; i++)
	{
		int size = rhs.m_neuron_layers[i].size();
		for (int j = 0; j < size; j++)
		{
			m_neuron_layers[i][j] = rhs.m_neuron_layers[i][j];
		}
	}
}

SnakeGame::SnakeGame() : m_apple_count(0), m_died_count(0), m_max_apple_count(0),  m_movements(0), m_moved_without_taking_apple(0), m_direction(0),
m_time_to_move(0.0f), m_timer(0.0f), m_increase_size(false), m_apple_position_r(0), m_apple_position_c(0), m_snake_died(false),
m_snake_took_apple(false), m_total_apple_count(0)
{
	m_board_witdh = g_terrain.GetWidth();
}

void SnakeGame::Initialize()
{
	//put the snake into the middle of the board
	g_terrain.SetColor(1, 1, DEBUG_COLOR_BLUE);
	m_snake.push_back({ 1, 1 });


	//apple position
	g_terrain.SetColor(1, 3, DEBUG_COLOR_RED);
	m_apple_position_r = 1;
	m_apple_position_c = 3;



	g_QLearning.m_row_distance = std::abs(1 - 1);
	g_QLearning.m_col_distance = std::abs(1 - 3);
}

void SnakeGame::Update()
{
	m_timer += g_clock.GetElapsedTime();

	bool apple_eaten = false;
	//the snake is ready to move
	if (m_timer > m_time_to_move)
	{
		//we choose the action that we are going to use
		g_QLearning.Update();
		
		m_moved_without_taking_apple++;


		int snake_r = m_snake[0].first;
		int snake_c = m_snake[0].second;

		m_timer = 0.0f;

		//apply snake movement 
		int size = m_snake.size();
		std::pair<int, int> last_position = m_snake[0];

		//first we move the head
		if (m_direction == 0) // up
		{
			m_snake[0].first++;
		}
		else if (m_direction == 1) // right
		{
			m_snake[0].second++;
		}
		else if (m_direction == 2) // down
		{
			m_snake[0].first--;
		}
		else // direction == 3 left
		{
			m_snake[0].second--;
		}

		//SNAKE DIED
		if (g_terrain.IsWall(m_snake[0].first, m_snake[0].second))
		{
			g_terrain.SetColor(last_position.first, last_position.second, DEBUG_COLOR_WHITE);
			//after updarting the snake we can know the next state that we are going to be and we can learn 
			m_snake_died = true;
			g_QLearning.Learns();
			Reset();
			return;
		}

		//we check if we ate an apple
		auto check_apple = g_terrain.GetColor(m_snake[0].first, m_snake[0].second);

		//SNAKE DIED
		if (check_apple == DEBUG_COLOR_GREEN && !(m_snake[0].first == m_snake[size - 1].first && m_snake[0].second == m_snake[size - 1].second))
		{
			g_terrain.SetColor(last_position.first, last_position.second, DEBUG_COLOR_WHITE);

			//after updarting the snake we can know the next state that we are going to be and we can learn 
			m_snake_died = true;
			g_QLearning.Learns();
			Reset();
			return;
		}
		if (check_apple == DEBUG_COLOR_RED)
		{
			m_apple_count++;
			m_total_apple_count++;
			if (m_apple_count > m_max_apple_count)
				m_max_apple_count = m_apple_count;
			m_increase_size = true;
			apple_eaten = true;
			if (size == 1)
			{
				m_snake.push_back(last_position);
				m_increase_size = false;
			}
		}
		//set the color as the snake color
		g_terrain.SetColor(m_snake[0].first, m_snake[0].second, DEBUG_COLOR_BLUE);
		g_terrain.SetColor(last_position.first, last_position.second, DEBUG_COLOR_WHITE);

		for (int i = 1; i < size; i++)
		{
			if (size - 1 == i  && m_increase_size)
			{
				m_increase_size = false;
				m_snake.push_back({ m_snake[i].first, m_snake[i].second });
			}
			else
			{
				if(g_terrain.GetColor(m_snake[i].first, m_snake[i].second) != DEBUG_COLOR_BLUE)
					g_terrain.SetColor(m_snake[i].first, m_snake[i].second, DEBUG_COLOR_WHITE);
			}

			auto save = m_snake[i];
			m_snake[i] = last_position;
			last_position = save;
			
			g_terrain.SetColor(m_snake[i].first, m_snake[i].second, DEBUG_COLOR_GREEN);

		}

		if (apple_eaten)
		{
			m_snake_took_apple = true;
			m_moved_without_taking_apple = 0;
			//position the apple into a random position
			bool positioned = false;
			while (!positioned)
			{
				int rand_r = g_random.RangeInt(0, m_board_witdh-1);
				int rand_c = g_random.RangeInt(0, m_board_witdh-1);

				if((rand_r != 1 || rand_c != 1) && (m_snake[size-1].first!= rand_r || m_snake[size-1].second != rand_c))
					if (g_terrain.GetColor(rand_r, rand_c) == DEBUG_COLOR_WHITE)
					{
						g_terrain.SetColor(rand_r, rand_c, DEBUG_COLOR_RED);
						positioned = true;
						m_apple_position_r = rand_r;
						m_apple_position_c = rand_c;
					}
			}
		}

		if (g_terrain.GetColor(m_apple_position_r, m_apple_position_c) != DEBUG_COLOR_RED 
			&& g_terrain.GetColor(m_apple_position_r, m_apple_position_c) == DEBUG_COLOR_WHITE)
		{
			g_terrain.SetColor(m_apple_position_r, m_apple_position_c, DEBUG_COLOR_RED);
		}

		//after updarting the snake we can know the next state that we are going to be and we can learn 
		g_QLearning.Learns();
	}

}

void SnakeGame::Reset()
{

	m_apple_count = 0;

	for (auto& it : m_snake)
	{
		if(!g_terrain.IsWall(it.first, it.second))
			g_terrain.SetColor(it.first, it.second, DEBUG_COLOR_WHITE);
	}
	m_snake.clear();

	m_died_count++;

	if (g_terrain.GetColor(1, 1) != DEBUG_COLOR_RED)
	{
		//put the snake into the middle of the board
		g_terrain.SetColor(1, 1, DEBUG_COLOR_BLUE);
		m_snake.push_back({ 1, 1 });
	}
	else
	{
		//put the snake into the middle of the board
		g_terrain.SetColor(2, 2, DEBUG_COLOR_BLUE);
		m_snake.push_back({ 2, 2 });
	}
}

void Q_Learning::Initialize()
{

	//HERE WE INITIALIZE THE NEURAL NETWORKS
	m_network_main.m_learning_rate = 0.00100f;
	//we dont put a learning rate here as this one is going to be updated by copying from main network
	m_network_target.m_learning_rate = 0.0f;

	//we will have 1 INPUT / 2 HIDEN / 1 OUTPUT
	m_network_main.m_number_of_layers   = 4;
	m_network_target.m_number_of_layers = 4;

	//we will have 12 different states sa input
	m_network_main.m_input_layer_size   = 12;
	m_network_target.m_input_layer_size = 12;

	//100 neurons in the 3 hiden layers
	m_network_main.m_hiden_layer_size   = 99;
	m_network_target.m_hiden_layer_size = 99;

	//4 direction for the input layer
	m_network_main.m_output_layer_size   = 4;
	m_network_target.m_output_layer_size = 4;

	m_network_target.Initialize();
	m_network_main.Initialize();


	m_network_target.m_real_outputs.resize(4);
	m_network_main.m_real_outputs.resize(4);
	//we reserve the size to avoid alocations in game
	m_reply_buffer.reserve(m_max_batch);


	//values that you can change in order to performe different learning

	//number of replays that we will appy per step
	m_max_batch = 1000;
	m_batch_tryes = 500;
	//to calculate the Q value
	m_future_reward_value = 0.95f;

	//for random exploration 
	m_epsilon_min = 0.001f;
	m_epsilon_decay = 0.001f;
	m_epsilon_value = 0.99f;

	//to update the target network
	m_iterations_to_update_target = 250;
	m_actual_iteration_numbers = 0;

	//Rewards when he goes to a new state

	m_died_reward					= -100.0f;
	m_got_apple_reward				=  50.0f;
	m_got_closer_from_apple_reward	=  5.0f;
	m_got_further_from_apple_reward	= -5.0f;

}

void Q_Learning::Update()
{
	//we first check if we need to update the target network
	m_actual_iteration_numbers++;
	if (m_actual_iteration_numbers > m_iterations_to_update_target)
	{
		m_actual_iteration_numbers = 0;
		m_network_target.CopyNetwork(m_network_main);
		m_updated_once = true;
	}

	CalculateState();
	
	//we then copy the states to the input layer of our neural network
	m_network_main.m_neuron_layers[0][0].m_output  = (float)m_apple_is_up;
	m_network_main.m_neuron_layers[0][1].m_output  = (float)m_apple_is_down;
	m_network_main.m_neuron_layers[0][2].m_output  = (float)m_apple_is_left;
	m_network_main.m_neuron_layers[0][3].m_output  = (float)m_apple_is_right;
	m_network_main.m_neuron_layers[0][4].m_output  = (float)m_obstacle_up;
	m_network_main.m_neuron_layers[0][5].m_output  = (float)m_obstacle_down;
	m_network_main.m_neuron_layers[0][6].m_output  = (float)m_obstacle_on_right;
	m_network_main.m_neuron_layers[0][7].m_output  = (float)m_obstacle_on_left;
	m_network_main.m_neuron_layers[0][8].m_output  = (float)m_direction_right;
	m_network_main.m_neuron_layers[0][9].m_output  = (float)m_direction_left;
	m_network_main.m_neuron_layers[0][10].m_output = (float)m_direction_up;
	m_network_main.m_neuron_layers[0][11].m_output = (float)m_direction_down;

	//here we call the forward propagation to get all the Q values of all actions
	m_network_main.ForwardPropagation();

	//we will see if the action if selected randomly or we exploit
	float random_number = g_random.RangeFloat(0.0f, 1.0f);
	if (random_number > m_epsilon_value)
	{
		int direction = 0;
		int max_direction = 0;
		float max_value = -D3D10_FLOAT32_MAX;
		//we take the action with greater average reward
		for (auto& it : m_network_main.m_neuron_layers[m_network_main.m_number_of_layers - 1])
		{
			if (it.m_output > max_value)
			{
				max_direction = direction;
				max_value = it.m_output;
			}
			direction++;
		}

		//we set the direction that the snake is going to take
		g_snakeGame.SetDirection(max_direction);
	}
	else // random action
	{
		g_snakeGame.SetDirection(g_random.RangeInt(0,3));
	}
	//update the epsilon if we need to 
	m_epsilon_value -= m_epsilon_decay;
	if (m_epsilon_value < m_epsilon_min)
		m_epsilon_value = m_epsilon_min;

	//LATER WE WILL CALCULATE THE REWARD AND USE THE REPLAY BUFFER

}

void Q_Learning::Learns()
{

	//when the action is selected, we calculate the reward
	int new_row_distance = std::abs(g_snakeGame.m_snake[0].first  - g_snakeGame.m_apple_position_r);
	int new_col_distance = std::abs(g_snakeGame.m_snake[0].second - g_snakeGame.m_apple_position_c);

	float reward = 0.0f;

	//reward if the snake has died -100
	if (g_snakeGame.m_snake_died || g_snakeGame.m_moved_without_taking_apple >= 50)
	{
		reward = m_died_reward;
		if (g_snakeGame.m_moved_without_taking_apple >= 50)
		{
			g_snakeGame.Reset();
			int new_row_distance = std::abs(g_snakeGame.m_snake[0].first - g_snakeGame.m_apple_position_r);
			int new_col_distance = std::abs(g_snakeGame.m_snake[0].second - g_snakeGame.m_apple_position_c);
			g_snakeGame.m_moved_without_taking_apple = 0;
		}

		m_row_distance = new_row_distance;
		m_col_distance = new_col_distance;
	}
	else
	{
		//+20 of reward if he takes the apple 
		if (g_snakeGame.m_snake_took_apple)
		{
			reward = m_got_apple_reward;
			g_snakeGame.m_snake_took_apple = false;
		}
		else
		{
			//+1 of reward if we are clsoser to the apple, -1 otherwise
			if (new_row_distance < m_row_distance || new_col_distance < m_col_distance)
			{
				reward = m_got_closer_from_apple_reward;
			}
			else
			{
				reward = m_got_further_from_apple_reward;
			}
			m_row_distance = new_row_distance;
			m_col_distance = new_col_distance;
		}
	}

	//here we do a backward propagation to learn what we did in the current action 

	float real_value;
	float max_value = 0.0f;
	//we first calculat the real output that it should get
	if (g_snakeGame.m_snake_died)//final state
	{
		real_value = reward;
	}
	else
	{
		if (m_updated_once)
		{


			//we then copy the states to the input layer of our neural network
			m_network_target.m_neuron_layers[0][0].m_output = (float)m_apple_is_up;
			m_network_target.m_neuron_layers[0][1].m_output = (float)m_apple_is_down;
			m_network_target.m_neuron_layers[0][2].m_output = (float)m_apple_is_left;
			m_network_target.m_neuron_layers[0][3].m_output = (float)m_apple_is_right;
			m_network_target.m_neuron_layers[0][4].m_output = (float)m_obstacle_up;
			m_network_target.m_neuron_layers[0][5].m_output = (float)m_obstacle_down;
			m_network_target.m_neuron_layers[0][6].m_output = (float)m_obstacle_on_right;
			m_network_target.m_neuron_layers[0][7].m_output = (float)m_obstacle_on_left;
			m_network_target.m_neuron_layers[0][8].m_output = (float)m_direction_right;
			m_network_target.m_neuron_layers[0][9].m_output = (float)m_direction_left;
			m_network_target.m_neuron_layers[0][10].m_output = (float)m_direction_up;
			m_network_target.m_neuron_layers[0][11].m_output = (float)m_direction_down;

			m_network_target.ForwardPropagation();


			max_value = -D3D10_FLOAT32_MAX;

			for (auto& it : m_network_target.m_neuron_layers[m_network_target.m_number_of_layers - 1])
			{
				if (it.m_output > max_value)
					max_value = it.m_output;
			}

			real_value = reward + m_future_reward_value * max_value;
		}
		else//we want the network at the beggining to learn a bit and later we will let to predict future
			real_value = reward;
	}
	int action_taken = g_snakeGame.GetDirection();

	m_network_main.m_real_outputs[0] = m_network_main.m_neuron_layers[m_network_main.m_number_of_layers-1][0].m_output;
	m_network_main.m_real_outputs[1] = m_network_main.m_neuron_layers[m_network_main.m_number_of_layers-1][1].m_output;
	m_network_main.m_real_outputs[2] = m_network_main.m_neuron_layers[m_network_main.m_number_of_layers-1][2].m_output;
	m_network_main.m_real_outputs[3] = m_network_main.m_neuron_layers[m_network_main.m_number_of_layers-1][3].m_output;
	m_network_main.m_real_outputs[action_taken] = real_value;

	m_network_main.BackwardPropagation();

	//here we replay the past experience that we have saved
	int current_state_id = 0;

	if (m_apple_is_up == 1)
		current_state_id |= 1;
	if(m_apple_is_down     == 1)
		current_state_id |= 2;
	if(m_apple_is_left     == 1)
		current_state_id |= 4;
	if(m_apple_is_right    == 1)
		current_state_id |= 8;
	if(m_obstacle_up       == 1)
		current_state_id |= 16;
	if(m_obstacle_down     == 1)
		current_state_id |= 32;
	if(m_obstacle_on_right == 1)
		current_state_id |= 64;
	if(m_obstacle_on_left  == 1)
		current_state_id |= 128;
	if(m_direction_right   == 1)
		current_state_id |= 256;
	if(m_direction_left    == 1)
		current_state_id |= 512;
	if(m_direction_up      == 1)
		current_state_id |= 1024;
	if(m_direction_down    == 1)
		current_state_id |= 2048;


	int start, end;
	//we choose if the replay is going to get random saved states
	// or if its gonna get all the buffer in case we dont have that many
	if ((int)m_reply_buffer.size() >= m_batch_tryes)
	{
		int random_bantches = g_random.RangeInt(0, m_reply_buffer.size() - m_batch_tryes);

		start = random_bantches;

		end = (random_bantches + m_batch_tryes);
	}
	else
	{
		start = 0;
		end = m_reply_buffer.size();
	}


	for (int i = start; i < end; i++)
	{
		//we first estimate the future reward that we could get
		float Q_value = 0.0f;
		if (!m_reply_buffer[i].final)
		{
			//we then copy the states to the input layer of our neural network
			if ((m_reply_buffer[i].m_next_state & 1) == 1)
				m_network_target.m_neuron_layers[0][0].m_output = 1.0f;
			else
				m_network_target.m_neuron_layers[0][0].m_output = 0.0f;

			if ((m_reply_buffer[i].m_next_state & 2) == 2)
				m_network_target.m_neuron_layers[0][1].m_output = 1.0f;
			else
				m_network_target.m_neuron_layers[0][1].m_output = 0.0f;

			if ((m_reply_buffer[i].m_next_state & 4) == 4)
				m_network_target.m_neuron_layers[0][2].m_output = 1.0f;
			else
				m_network_target.m_neuron_layers[0][2].m_output = 0.0f;

			if ((m_reply_buffer[i].m_next_state & 8) == 8)
				m_network_target.m_neuron_layers[0][3].m_output = 1.0f;
			else
				m_network_target.m_neuron_layers[0][3].m_output = 0.0f;

			if ((m_reply_buffer[i].m_next_state & 16) == 16)
				m_network_target.m_neuron_layers[0][4].m_output = 1.0f;
			else
				m_network_target.m_neuron_layers[0][4].m_output = 0.0f;

			if ((m_reply_buffer[i].m_next_state & 32) == 32)
				m_network_target.m_neuron_layers[0][5].m_output = 1.0f;
			else
				m_network_target.m_neuron_layers[0][5].m_output = 0.0f;

			if ((m_reply_buffer[i].m_next_state & 64) == 64)
				m_network_target.m_neuron_layers[0][6].m_output = 1.0f;
			else
				m_network_target.m_neuron_layers[0][6].m_output = 0.0f;

			if ((m_reply_buffer[i].m_next_state & 128) == 128)
				m_network_target.m_neuron_layers[0][7].m_output = 1.0f;
			else
				m_network_target.m_neuron_layers[0][7].m_output = 0.0f;

			if ((m_reply_buffer[i].m_next_state & 256) == 256)
				m_network_target.m_neuron_layers[0][8].m_output = 1.0f;
			else
				m_network_target.m_neuron_layers[0][8].m_output = 0.0f;

			if ((m_reply_buffer[i].m_next_state & 512) == 512)
				m_network_target.m_neuron_layers[0][9].m_output = 1.0f;
			else
				m_network_target.m_neuron_layers[0][9].m_output = 0.0f;

			if ((m_reply_buffer[i].m_next_state & 1024) == 1024)
				m_network_target.m_neuron_layers[0][10].m_output = 1.0f;
			else
				m_network_target.m_neuron_layers[0][10].m_output = 0.0f;

			if ((m_reply_buffer[i].m_next_state & 2048) == 2048)
				m_network_target.m_neuron_layers[0][11].m_output = 1.0f;
			else
				m_network_target.m_neuron_layers[0][11].m_output = 0.0f;

			m_network_target.ForwardPropagation();

			max_value = -D3D10_FLOAT32_MAX;

			for (auto& it : m_network_target.m_neuron_layers[m_network_target.m_number_of_layers - 1])
			{
				if (it.m_output > max_value)
					max_value = it.m_output;
			}

			Q_value = m_reply_buffer[i].m_reward + m_future_reward_value * max_value;

		}
		else
			Q_value = m_reply_buffer[i].m_reward;


		//we then copy the states to the input layer of our neural network
		if ((m_reply_buffer[i].m_state & 1) == 1)
			m_network_main.m_neuron_layers[0][0].m_output = 1.0f;
		else
			m_network_main.m_neuron_layers[0][0].m_output = 0.0f;

		if ((m_reply_buffer[i].m_state & 2) == 2)
			m_network_main.m_neuron_layers[0][1].m_output = 1.0f;
		else
			m_network_main.m_neuron_layers[0][1].m_output = 0.0f;

		if ((m_reply_buffer[i].m_state & 4) == 4)
			m_network_main.m_neuron_layers[0][2].m_output = 1.0f;
		else
			m_network_main.m_neuron_layers[0][2].m_output = 0.0f;

		if ((m_reply_buffer[i].m_state & 8) == 8)
			m_network_main.m_neuron_layers[0][3].m_output = 1.0f;
		else
			m_network_main.m_neuron_layers[0][3].m_output = 0.0f;

		if ((m_reply_buffer[i].m_state & 16) == 16)
			m_network_main.m_neuron_layers[0][4].m_output = 1.0f;
		else
			m_network_main.m_neuron_layers[0][4].m_output = 0.0f;

		if ((m_reply_buffer[i].m_state & 32) == 32)
			m_network_main.m_neuron_layers[0][5].m_output = 1.0f;
		else
			m_network_main.m_neuron_layers[0][5].m_output = 0.0f;

		if ((m_reply_buffer[i].m_state & 64) == 64)
			m_network_main.m_neuron_layers[0][6].m_output = 1.0f;
		else
			m_network_main.m_neuron_layers[0][6].m_output = 0.0f;

		if ((m_reply_buffer[i].m_state & 128) == 128)
			m_network_main.m_neuron_layers[0][7].m_output = 1.0f;
		else
			m_network_main.m_neuron_layers[0][7].m_output = 0.0f;

		if ((m_reply_buffer[i].m_state & 256) == 256)
			m_network_main.m_neuron_layers[0][8].m_output = 1.0f;
		else
			m_network_main.m_neuron_layers[0][8].m_output = 0.0f;

		if ((m_reply_buffer[i].m_state & 512) == 512)
			m_network_main.m_neuron_layers[0][9].m_output = 1.0f;
		else
			m_network_main.m_neuron_layers[0][9].m_output = 0.0f;

		if ((m_reply_buffer[i].m_state & 1024) == 1024)
			m_network_main.m_neuron_layers[0][10].m_output = 1.0f;
		else
			m_network_main.m_neuron_layers[0][10].m_output = 0.0f;

		if ((m_reply_buffer[i].m_state & 2048) == 2048)
			m_network_main.m_neuron_layers[0][11].m_output = 1.0f;
		else
			m_network_main.m_neuron_layers[0][11].m_output = 0.0f;

		m_network_main.ForwardPropagation();


		m_network_main.m_real_outputs[0] = m_network_main.m_neuron_layers[m_network_main.m_number_of_layers - 1][0].m_output;
		m_network_main.m_real_outputs[1] = m_network_main.m_neuron_layers[m_network_main.m_number_of_layers - 1][1].m_output;
		m_network_main.m_real_outputs[2] = m_network_main.m_neuron_layers[m_network_main.m_number_of_layers - 1][2].m_output;
		m_network_main.m_real_outputs[3] = m_network_main.m_neuron_layers[m_network_main.m_number_of_layers - 1][3].m_output;
		m_network_main.m_real_outputs[m_reply_buffer[i].m_action] = Q_value;

		m_network_main.BackwardPropagation();
	}

	//after all, we save the current state in the replay buffer
	if ((int)m_reply_buffer.size() < m_max_batch)
	{
		ExperienceInfo new_experience;

		CalculateState();

		int next_state_id = 0;

		if (m_apple_is_up == 1)
			next_state_id |= 1;
		if (m_apple_is_down == 1)
			next_state_id |= 2;
		if (m_apple_is_left == 1)
			next_state_id |= 4;
		if (m_apple_is_right == 1)
			next_state_id |= 8;
		if (m_obstacle_up == 1)
			next_state_id |= 16;
		if (m_obstacle_down == 1)
			next_state_id |= 32;
		if (m_obstacle_on_right == 1)
			next_state_id |= 64;
		if (m_obstacle_on_left == 1)
			next_state_id |= 128;
		if (m_direction_right == 1)
			next_state_id |= 256;
		if (m_direction_left == 1)
			next_state_id |= 512;
		if (m_direction_up == 1)
			next_state_id |= 1024;
		if (m_direction_down == 1)
			next_state_id |= 2048;

		new_experience.m_state = current_state_id;
		new_experience.m_action = action_taken;
		new_experience.m_next_state = next_state_id;
		new_experience.m_reward = reward;
		if (g_snakeGame.m_snake_died)
		{
			g_snakeGame.m_snake_died = false;
			new_experience.final = true;
		}
		else
			new_experience.final = false;

		m_reply_buffer.push_back(new_experience);
	}
}

void Q_Learning::CalculateState()
{
	int snake_r = g_snakeGame.m_snake[0].first;
	int snake_c = g_snakeGame.m_snake[0].second;

	//we first get the state status that we are going to use
	//we check the states of where the apple is positioned
	if (snake_r < g_snakeGame.m_apple_position_r)
	{
		m_apple_is_up = 1;
		m_apple_is_down = 0;
	}
	else
	{
		m_apple_is_up = 0;
		m_apple_is_down = 1;
	}

	if (snake_c < g_snakeGame.m_apple_position_c)
	{
		m_apple_is_left = 0;
		m_apple_is_right = 1;
	}
	else
	{
		m_apple_is_left = 1;
		m_apple_is_right = 0;
	}
	//we check the state if we have an obstacle
	if (g_terrain.IsWall(snake_r + 1, snake_c) || g_terrain.GetColor(snake_r + 1, snake_c) == DEBUG_COLOR_GREEN)
		m_obstacle_up = 1;
	else
		m_obstacle_up = 0;

	if (g_terrain.IsWall(snake_r - 1, snake_c) || g_terrain.GetColor(snake_r - 1, snake_c) == DEBUG_COLOR_GREEN)
		m_obstacle_down = 1;
	else
		m_obstacle_down = 0;

	if (g_terrain.IsWall(snake_r, snake_c + 1) || g_terrain.GetColor(snake_r, snake_c + 1) == DEBUG_COLOR_GREEN)
		m_obstacle_on_right = 1;
	else
		m_obstacle_on_right = 0;

	if (g_terrain.IsWall(snake_r, snake_c - 1) || g_terrain.GetColor(snake_r, snake_c - 1) == DEBUG_COLOR_GREEN)
		m_obstacle_on_left = 1;
	else
		m_obstacle_on_left = 0;

	int snake_direction = g_snakeGame.GetDirection();

	if (snake_direction == 0)
	{
		m_direction_right = 0;
		m_direction_left = 0;
		m_direction_up = 1;
		m_direction_down = 0;
	}
	else if (snake_direction == 1)
	{
		m_direction_right = 1;
		m_direction_left = 0;
		m_direction_up = 0;
		m_direction_down = 0;
	}
	else if (snake_direction == 2)
	{
		m_direction_right = 0;
		m_direction_left = 0;
		m_direction_up = 0;
		m_direction_down = 1;
	}
	else
	{
		m_direction_right = 0;
		m_direction_left = 1;
		m_direction_up = 0;
		m_direction_down = 0;
	}
}
