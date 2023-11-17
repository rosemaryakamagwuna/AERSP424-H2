#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <random>
#include "HW2_Visualizer.h"

// Question 1
class FlightContainer {
private:
    std::unordered_map<std::string, std::unordered_map<std::string, int> > flights;

public:
    FlightContainer() {
        flights["SCE"]["PHL"] = 160;
        flights["SCE"]["ORD"] = 640;
        flights["SCE"]["EWR"] = 220;
    }

    int getFlightDistance(const std::string& from, const std::string& to) const {
        return flights.at(from).at(to);
    }
};

// Question 2
class Plane {
protected:
    double wait_time;

private:
    FlightContainer flightContainer;
    double pos;
    double vel;
    double distance;
    double loiter_time;
    bool at_SCE;
    std::string origin;
    std::string destination;

public:
    // Constructor
    Plane(const std::string& from, const std::string& to)
        : origin(from), destination(to), flightContainer(),
          distance(flightContainer.getFlightDistance(from, to)),
          pos(0.0), vel(0.0), wait_time(0.0), loiter_time(0.0), at_SCE(true) {}

    // Destructor
    virtual ~Plane() {}

    // Operate function with flowchart logic
    void operate(double dt);

    // Getter functions
    double getPos() const { return pos; }
    double getVel() const { return vel; }
    double getWaitTime() const { return wait_time; }
    double getLoiterTime() const { return loiter_time; }
    std::string getOrigin() const { return origin; }
    std::string getDestination() const { return destination; }
    bool getAtSCE() const { return at_SCE; }

    // Setter functions
    void setVel(double newVel) { vel = newVel; }
    void setLoiterTime(double newLoiterTime) { loiter_time = newLoiterTime; }

    // Function to calculate distance to SCE
    double distanceToSCE() const;

    // Virtual functions
    virtual double timeOnGround() const = 0;
    virtual std::string planeType() const = 0;

    // Static function to draw from a normal distribution
    static double drawFromNormalDist(double m, double sd);
protected:
    
    void setWaitTime(double newWaitTime) const {
        const_cast<Plane*>(this)->wait_time = newWaitTime;
    }
};

// Operate function implementation
void Plane::operate(double dt) {
    if (loiter_time != 0) {
        loiter_time -= dt;
        if (loiter_time < 0) {
            loiter_time = 0;
        }
    } else if (wait_time != 0) {
        wait_time -= dt;
        if (wait_time < 0) {
            wait_time = 0;
        }
    } else if (pos < distance) {
        pos += vel * dt;
        at_SCE = false;
    } else if (destination == "SCE") {
        at_SCE = true;
        timeOnGround();
        std::swap(origin, destination);
        pos = 0.0;
    } else {
        timeOnGround();
        std::swap(origin, destination);
        pos = 0.0;
    }
}

// Function to calculate distance to SCE
double Plane::distanceToSCE() const {
    return (destination == "SCE") ? (distance - pos) : 0.0;
}

// Static function to draw from a normal distribution
double Plane::drawFromNormalDist(double m, double sd) {
    std::random_device rd{};
    std::mt19937 gen{ rd() };
    std::normal_distribution<> d{ m, sd };
    return d(gen);
}

// Concrete implementation of Plane class
class GAPlane : public Plane {
public:
    GAPlane(const std::string& from, const std::string& to) : Plane(from, to) {}

    double timeOnGround() const override {
        return wait_time;
    }

    std::string planeType() const override {
        return "GA";
    }
};

// Question 4

// Airliner class
class Airliner : public Plane {
private:
    std::string Airline;

public:
    // Constructor
    Airliner(const std::string& airline, const std::string& from, const std::string& to)
        : Plane(from, to), Airline(airline) {}

    // Destructor
    ~Airliner() {}

    // Overridden function
    double timeOnGround() const override {
        // Draw wait_time from normal distribution with mean 1800 and standard deviation 600
        double newWaitTime = drawFromNormalDist(1800.0, 600.0);
        if (newWaitTime < 0.0) {
            newWaitTime = 0.0;
        }
        setWaitTime(newWaitTime);
        return wait_time;
    }

    std::string planeType() const override {
        return Airline;
    }
};

// GeneralAviation class
class GeneralAviation : public Plane {
public:
    // Constructor
    GeneralAviation(const std::string& from, const std::string& to)
        : Plane(from, to) {}

    // Destructor
    ~GeneralAviation() {}

    // Overridden function
    double timeOnGround() const override {
        // Draw wait_time from normal distribution with mean 600 and standard deviation 60
        double newWaitTime = drawFromNormalDist(600.0, 60.0);
        if (newWaitTime < 0.0) {
            newWaitTime = 0.0;
        }
        setWaitTime(newWaitTime);
        return wait_time;
    }

    std::string planeType() const override {
        return "GA";
    }
};

// Question 6

// ATC class
class ATC {
private:
    std::vector<Plane*> registered_planes;
    const int MAX_LANDED_PLANE_NUM = 2;
    const int AIRSPACE_DISTANCE = 50;

public:
    ATC() {}

    ~ATC() {}

    void registerPlane(Plane* plane) {
        registered_planes.push_back(plane);
    }

    void controlTraffic() {
        int landed_planes = 0;
        for (int i = 0; i < registered_planes.size(); ++i) {
            landed_planes += registered_planes[i]->getAtSCE();
        };
            if (landed_planes >= MAX_LANDED_PLANE_NUM) {
                
            

            for (int i = 0; i < registered_planes.size();++i) {
                if ((registered_planes[i]->getAtSCE() == 0 &&
                      registered_planes[i]->distanceToSCE() <= AIRSPACE_DISTANCE &&
                      registered_planes[i]->getLoiterTime() == 0)) {
                      registered_planes[i]->setLoiterTime(100);
                } 
            }
            }
    }
};

int main(int argc, char** argv) {
//Initializa HW2_VIZ
    HW2_VIZ viz;

    // Instantiate ATC
    ATC atc;

    // Question 5
    Airliner aa1("AA", "SCE", "PHL");
    aa1.setVel(0.13);
    aa1.setLoiterTime(2);
    atc.registerPlane(&aa1);

    Airliner ua1("UA", "SCE", "ORD");
    ua1.setVel(0.143);
    ua1.timeOnGround();
    atc.registerPlane(&ua1);

    Airliner ua2("UA", "SCE", "EWR");
    ua2.setVel(0.133);
    atc.registerPlane(&ua2);

    Airliner aa2("AA", "SCE", "ORD");
    aa2.setVel(0.139);
    aa2.timeOnGround();
    atc.registerPlane(&aa2);

    GeneralAviation ga1("SCE", "PHL");
    ga1.setVel(0.039);
    atc.registerPlane(&ga1);

    GeneralAviation ga2("SCE", "EWR");
    ga2.setVel(0.044);
    ga2.setLoiterTime(10);
    atc.registerPlane(&ga2);

    GeneralAviation ga3("SCE", "ORD");
    ga3.setVel(0.05);
    atc.registerPlane(&ga3);

    // Simulation loop
    double timestep = 100;  // Choose a timestep between [10, 100]
    double totaltime = 0;
    double max_totaltime = 5000;

    while (!viz.close()) {
       
        // std::cout << "Total time: " << totaltime << std::endl;

        // // Print out the position of all airplanes at each timestep
        // std::cout << "AA1 Position: " << aa1.getPos() << " miles" << std::endl;
        // std::cout << "UA1 Position: " << ua1.getPos() << " miles" << std::endl;
        // std::cout << "UA2 Position: " << ua2.getPos() << " miles" << std::endl;
        // std::cout << "AA2 Position: " << aa2.getPos() << " miles" << std::endl;
        // std::cout << "GA1 Position: " << ga1.getPos() << " miles" << std::endl;
        // std::cout << "GA2 Position: " << ga2.getPos() << " miles" << std::endl;
        // std::cout << "GA3 Position: " << ga3.getPos() << " miles" << std::endl;
        // std::cout << "" << std::endl;

        // Operate and print out the position of all airplanes at each timestep
        aa1.operate(timestep);
        ua1.operate(timestep);
        ua2.operate(timestep);
        aa2.operate(timestep);
        ga1.operate(timestep);
        ga2.operate(timestep);
        ga3.operate(timestep);

        // Control traffic using ATC
        atc.controlTraffic();
        

        viz.visualize_plane(aa1.planeType(), aa1.getOrigin(),  aa1.getDestination(), aa1.getPos());
        viz.visualize_plane(ua1.planeType(), ua1.getOrigin(),  ua1.getDestination(), ua1.getPos());
        viz.visualize_plane(ua2.planeType(), ua2.getOrigin(),  ua2.getDestination(), ua2.getPos());
        viz.visualize_plane(aa2.planeType(), aa2.getOrigin(),  aa2.getDestination(), aa2.getPos());
        viz.visualize_plane(ga1.planeType(), ga1.getOrigin(),  ga1.getDestination(), ga1.getPos());
        viz.visualize_plane(ga2.planeType(), ga2.getOrigin(),  ga2.getDestination(), ga2.getPos());
        viz.visualize_plane(ga3.planeType(), ga3.getOrigin(),  ga3.getDestination(), ga3.getPos());

/*       if (totaltime >= max_totaltime){
        break;

      };

      totaltime += timestep; */
      viz.update (timestep);  
    }

    return 0;
}
