#include<bits/stdc++.h>
using namespace std;

#define MAX 1000

class Data{
    friend class LocationsMap;
    friend class User;
    vector<vector<pair<long long,bool>>> map; // Adjacency Matrix to Represent Vertices(Locations) first template is used to insert Distances second template is used to ensure Traffic True means High Traffic, False means Low Traffic
    unordered_map<string,long long> mp;  // To Store Indexes of Every Location from the Adjacency Matrix with respect to the Names of Location
    long long numLocations;
    Data()
    {
        map.resize(MAX,vector<pair<long long,bool>>(MAX,{0,false}));

        map[1][2]={1,false};
        map[2][1]={1,false};
        map[1][3]={4,false};
        map[3][1]={4,false};
        map[1][10]={10,false};
        map[10][1]={10,false};
        map[2][3]={1,false};
        map[3][2]={1,false};
        map[3][4]={2,false};
        map[4][3]={2,false};
        map[5][4]={3,false};
        map[4][5]={3,false};
        map[4][9]={3,false};
        map[9][4]={3,false};
        map[4][10]={7,false};
        map[6][7]={3,false};
        map[7][6]={3,false};
        map[10][4]={7,false};
        map[5][6]={1,false};
        map[6][5]={1,false};
        map[7][8]={3,false};
        map[8][7]={3,false};

        numLocations=10;
        mp["Maskan"]=1;
        mp["Shahrah-e-Faisal"]=4;
        mp["NIPA"]=3;
        mp["Gulshan Roundabout"]=2;
        mp["Malir Cantt"]=5;
        mp["Anwar Baloch"]=6;
        mp["Shah Latif Town"]=7;
        mp["FAST-NUCES"]=8;
        mp["PECHS"]=9;
        mp["Defence"]=10;
    }
};

class LocationsMap{
    protected:
    static Data data;
    public:
    LocationsMap() {}

    void addNewLocation(vector<pair<long long,string>> links,string newLocation) // distances, linked Locations
    {
        if(data.numLocations==MAX-1)
        {
            cout<<"Cannot Add More Locations\n";
            return;
        }

        for(auto& location : data.mp)
        {
            if(location.first==newLocation)
            {
                cout<<"Location Already Exists\n";
                return;
            }
        }

        for(int i=0;i<links.size();++i)
        {
            if(newLocation==links[i].second)
            {
                cout<<"Cannot Link Location with Itself\n";
                return;
            }
        }

        if(data.numLocations==1)
        {
            data.mp[newLocation]=data.numLocations++;
            cout<<"Successfully Inserted : "<<newLocation<<endl;
            return;
        }

        if(links.empty() && data.numLocations!=1)
        {
            cout<<"Cannot Add Location without any Link\n";
            return;
        }
        data.mp[newLocation]=data.numLocations;
        vector<bool> isValid(links.size(),true);
        cout<<"New Location Added Successfully between : "<<newLocation<<endl;
        cout<<"--------------------------------------------------------"<<endl;
        for(int i=1;i<=links.size();++i) // We will start inserting locations from index 1 of rows and columns of adjacency matrix to avoid the index 0 problem in hashmap of the source during searching
        {
            if(data.mp[links[i-1].second]!=0)
            {
                data.map[data.mp[links[i-1].second]][data.numLocations].first=links[i-1].first;
                data.map[data.numLocations][data.mp[links[i-1].second]].first=links[i-1].first;
            }
            else
            {
                cout<<links[i-1].second<<" Not Found\n";
                isValid[i-1]=false;
            }
        }
        ++data.numLocations;
        for(int i=0;i<links.size();++i)
        {
            if(isValid[i])
            {
                cout<<links[i].second<<endl;
            }
        }
        cout<<"--------------------------------------------------------"<<endl;
    }

    void updateLocationName(string previousName,string newName)
    {
        if(data.mp[previousName]==0)
        {
            cout<<previousName<<" doesn't Exist\n";
            return;
        }
        long long index=data.mp[previousName];
        data.mp[previousName]=0;
        data.mp[newName]=index;
        cout<<"Successfully updated name of Location "<<previousName<<" to "<<newName<<endl;
    }

    void setTraffic(string source,string destination,bool status)
    {
        data.map[data.mp[source]][data.mp[destination]].second=status;
        data.map[data.mp[destination]][data.mp[source]].second=status;
    }
};

Data LocationsMap::data;

class User:public LocationsMap{
    public:
    User() {}

    void getShortestPath(string source,string destination) // Implementation of Dijkstra's Algorithm
    {
        if(data.mp.find(source)==data.mp.end() || data.mp.find(destination)==data.mp.end())
        {
            cout<<(data.mp.find(source)==data.mp.end() ? "Invalid Input for Source\n" : "Invalid Input for Destination\n");
            return;
        }
        int src=data.mp[source];
        int des=data.mp[destination];

        vector<long long> distance(data.numLocations+1,INT_MAX);
        vector<bool> visited(data.numLocations+1,false);
        vector<int> parent(data.numLocations+1,-1);

        distance[src]=0;
        for(int v=1;v<=data.numLocations;++v)
        {
            int u=-1;
            for(int i=1;i<=data.numLocations;++i)
            {
                if(!visited[i] && (u==-1 || distance[i]<distance[u]))
                {
                    u=i;
                }
            }

            if(u==-1)
            break;

            visited[u]=true;
            for(int V=1;V<=data.numLocations;++V)
            {
                if(data.map[u][V].first!=0 && !visited[V] && distance[u]+data.map[u][V].first<distance[V])
                {
                    distance[V]=distance[u]+data.map[u][V].first;
                    parent[V]=u;
                }
            }
        }

        if(distance[des]==INT_MAX)
        {
            cout<<"Path does not exist between "<<source<<" and "<<destination<<endl;
            return;
        }
        cout<<"Shortest Distance from "<<source<<" to "<<destination<<" is : "<<distance[des]<<" km\n";

        vector<long long> path;
        int current=des;
        while(current!=-1)
        {
            path.insert(path.begin(),current);
            current=parent[current];
        }

        vector<string> route(data.numLocations+1);
        for(auto& location : data.mp)
        {
            route[location.second]=location.first;
        }

        cout<<"Route is : \n";
        for(int i=0;i<path.size();++i)
        {
            cout<<route[path[i]];
            if(i<path.size()-1)
            cout<<"->";
        }
        cout<<endl;
    }
};

int main()
{
    User user;
    LocationsMap officer;
    char choice='5';
    while(choice!='0')
    {
        char select;
        cout<<"Press (1) for Developer Login\n";
        cout<<"Press (2) for User\n";
        cout<<"Press (0) to End Program\n";
        cin>>select;
        switch(select)
        {
            case '1':
            {
                fstream file;
                file.open("database.txt",ios::in);
                if(!file.is_open())
                {
                    cout<<"Unable to Login\n";
                    break;
                }
                string ID;
                string password;
                cout<<"Enter Developers ID : ";
                cin>>ID;
                cout<<"Enter password : ";
                cin>>password;
                string line;
                string dataID;
                string dataPassword;
                bool isValid=false;
                while(getline(file,line))
                {
                    int index=0;
                    for(int i=0;i<line.size();++i)
                    {
                        if(line[i]==' ')
                        break;
                        ++index;
                    }
                    dataID=line.substr(0,index);
                    dataPassword=line.substr(index+1,line.size());

                    if(dataID==ID && dataPassword==password)
                    {
                        isValid=true;
                        break;
                    }
                }
                file.close();
                if(!isValid)
                {
                    cout<<"Invalid Developer-ID (or) Password\n";
                    break;
                }

                char options='a';
                cout<<"Press (1) to Add New Location\n";
                cout<<"Press (2) to Update Location's Name\n";
                cout<<"Press (3) to Set Traffic\n";
                cout<<"Press (0) to Go Back\n";
                cin>>options;
                switch(options)
                {
                    case '1':
                    {
                        string newLocation;
                        cout<<"Enter New Location Name : ";
                        cin.ignore();
                        getline(cin,newLocation);
                        long long numLocations;
                        cout<<"Enter Number of Locations to link : ";
                        cin>> numLocations;
                        vector<pair<long long,string>> links(numLocations);
                        for(int i=0;i<links.size();++i)
                        {
                            cout<<"Enter Name of Linking Location : ";
                            cin.ignore();
                            getline(cin,links[i].second);
                            cout<<"Enter Distance between the Links "<<newLocation<<" and "<<links[i].second<<" : ";
                            cin>>links[i].first;
                        }
                        officer.addNewLocation(links,newLocation);
                        break;
                    }

                    case '2':
                    {
                        string oldName;
                        string newName;
                        cout<<"Enter the Location's Name to be Updated : ";
                        cin.ignore();
                        getline(cin,oldName);
                        cout<<"Enter the New Name for this Location  : ";
                        getline(cin,newName);
                        officer.updateLocationName(oldName,newName);
                        break;
                    }

                    case '3':
                    {
                        string source;
                        string destination;
                        cout<<"Enter Source to Update Traffic Status : ";
                        cin.ignore();
                        getline(cin,source);
                        cout<<"Enter Destination to Update Status : ";
                        getline(cin,destination);
                        bool status;
                        cout<<"Enter Status to Update between "<<source<<" and "<<destination<<" : ";
                        cin>>status;
                        officer.setTraffic(source,destination,status);
                        break;
                    }

                    default:
                    cout<<"Invalid Input\n";
                }
                break;
            }

            case '2':
            {
                string source;
                string destination;
                cout<<"Enter Source to Get the Shorted Route : ";
                cin.ignore();
                getline(cin,source);
                cout<<"Enter Destination to Reach : ";
                getline(cin,destination);
                user.getShortestPath(source,destination);
                break;
            }

            case '0':
            return 0;

            default:
            cout<<"Invalid Input\n";
        }
    }
    return 0;
}