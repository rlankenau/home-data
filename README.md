# HomeData Project

This project is intended to gather information about my home and garden, aimed at eventually tracking the temperature and humidity inside our hoop houses in the winter.

Data collection is done with low-power Arduino pro mini microcontrollers.  Most are currently running off of wall warts, but the planned instrument shelter will be powered by solar.

Data is transmitted via 2.4 GHz radio at 250 Kbps via a mesh network.  The networking stack uses the MySensors framework, running on the data collection nodes, and communicating using NRF24L01 radios.  The data is aggregated on a server running Ubuntu 17.04, where it is translated into the InfluxDB line protocol and ingested via a Telegraf socket listener.

Display currently uses Grafana, although I may eventually build custom dashboards for specific views.
