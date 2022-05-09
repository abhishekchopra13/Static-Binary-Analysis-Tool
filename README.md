# Static-Binary-Analysis-Tool

Running instructions

pip3 install -r requirements.txt

g++ taint_tracker.cpp -o taint_tracker

./taint_tracker ./sample

python3 graph_visualiser.py

python3 cfg_gen.py
