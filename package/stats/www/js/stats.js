plugInModule.controller('statsCtrl',function($scope,$http,$log){
	
	$scope.interfaces={"values": ["","","","","","",""]};
	$scope.activeInt={};
	$scope.running={};
	$scope.hosts=[];
	$scope.images={init:["no_data_graph.jpg","no_data_graph.jpg","no_data_graph.jpg","no_data_graph.jpg","no_data_graph.jpg"]};
	$scope.stats={interface:"eth0", images: ["","","","",""]}
	$scope.hostsbck={"values":[],"hostfile":"Current","int":"eth0"};
	$scope.donegenerating=false;
	$scope.stats.images = $scope.images.init;
	$scope.graphReady = false;
	$scope.testhostfile="";

	$scope.setMenuInterfaces = function(){
		var i;
		var arr = [$scope.activeInt.num];
		for (i=0; i<$scope.activeInt.num; i++){
			arr[i]=$scope.activeInt[i];
}
                $scope.interfaces.values = arr;
        };
        
	$scope.getInterfaces = function(){
	        $http.get('/api/libcg/simple_json_get_interfaces_backend').success(function(data){
					$scope.activeInt = data;
					$scope.setMenuInterfaces();
		});
	};

	$scope.getInterfaces();
	
	$scope.setHostValues = function(data){
		$scope.running = {};
		$scope.hosts = [];
		$scope.running = data[0];                                                                   
	        var i;                                                                                      
	        for(i=1;i<data.length;i++){                                                                 
	                $scope.hosts[i-1] = data[i];                                                        
                }                                                                                           
	};
	
	$scope.getHosts = function(){
		$http.get('/api/libcg/simple_json_get_hosts_backend').success(function(data){
			$scope.setHostValues(data);
		});
	};

	$scope.getHostsFiles = function(){
		$scope.graphReady = false;
		$http.get('/api/libcg/simple_json_get_hostsfiles_backend').success(function(data){
			$scope.hostsbck.int=$scope.stats.interface;
			var i;
			var arr = [data.length]
			for (i=0;i<data.length;i++){
				arr[i]=data[i][i];
			}
			$scope.hostsbck.values = arr;
			$http.post('/api/libcg/simple_json_post_numOfHosts_backend',$scope.hostsbck).success(function(data){
				var i;
				var labels = [];
				var vals = [];
				for (i=0;i<data.length;i++) {
					for (var key in data[i]){
						var num = data[i][key]
						if(key == "Current")
						{
							var d = new Date();                                                 
	                                                key=d.toISOString().slice(0,10);
						}
						labels.push(key);
						vals.push(num)
					}
				}
				labels.push(labels.shift());
				vals.push(vals.shift());
				$scope.barGraph.labels = labels;
				$scope.barGraph.data = vals;
				$scope.graphReady = true;
			});
                });
	};
	
	$scope.hostsUpdate = function(){
		if($scope.hostsbck.hostfile == "Current")
		{
			$scope.getHosts();
		}
		else
		{
			$http.post('/api/libcg/simple_json_post_bck_file_backend',$scope.hostsbck).success(function(data){
			        $scope.setHostValues(data);
        	        }); 
        	}
	};

	$scope.statsGenerate = function(){
		$scope.stats.images = $scope.images.init;
		$scope.hostsbck.hostfile="Current";
		$scope.running={};                                                                                  
                $scope.hosts=[];
		$scope.barGraph={};
		$scope.graphReady=false;
		$scope.donegenerating=false;
		$http.post('/api/libcg/simple_json_post_generate_backend',$scope.stats).success(function(data){
			$scope.stats=data;
			$scope.getHosts();
			$scope.getHostsFiles();
			$scope.donegenerating=true;
		});
	};
	
	$scope.statsGenerate();
	
	$scope.resetintstats = function(){                                                                     
                $log.log("Calling reset interfaces statistics");
		$http.get('/api/libcg/simple_json_reset_int_stats_backend').success(function(data){
			$scope.statsGenerate();
                });
        };                                                                                                     
                                                                                                               
        $scope.resethosts = function(){                                                                        
                $log.log("Calling reset hosts statistics");
		$http.get('/api/libcg/simple_json_reset_hosts_stats_backend').success(function(data){
			$scope.statsGenerate();
                });
        };
})
.directive("mgChart",function(){
	var myBarChart=null;

	var linkFunction = function(scope,element){                                                                 
                scope.$watch('graphReady',function(newVal, oldVal){                                                 
                        if (newVal && newVal !== oldVal)                                                            
                        {
				var data = {                                                                                                
			            labels: [],                                                                                             
			            datasets: [                                                                                             
			                {                                                                                                   
			                    label: "First dataset",                                                                         
			                    fillColor: "rgba(146,207,0,1.5)",                                                               
			                    strokeColor: "rgba(146,207,0,1.8)",                                                             
			                    highlightFill: "rgba(146,207,0,1.75)",                                                          
			                    highlightStroke: "rgba(146,207,0,1)",                                                           
			                    data: []                                                                                        
			                }                                                                                                   
			            ]                                                                                                       
			        };

				if(myBarChart!=null){
					myBarChart.destroy();
				}

                                console.log("Ready for bar graph"); 
                                var paragraph = element;                                                            
                                //$(paragraph).html('');
				data.labels = scope.barGraph.labels;
				data.datasets[0].data = scope.barGraph.data;
				var ctx = $(paragraph).get(0).getContext("2d");
				myBarChart = new Chart(ctx).Bar(data);
                        }                                                                                           
                });                                                                                                 
        };                                                                                                          
                                                                                                                    
        return {                                                                                                    
                restrict: 'A',                                                                                      
                link: linkFunction                                                                                 
        };
})
.directive("mgSelect",function(){
	var linkFunction = function(scope,element){
		scope.$watch('graphReady',function(newVal,oldVal){
			if(newVal && newVal !== oldVal)
			{
				console.log("Ready for calendar");
				
				var min = scope.hostsbck.values.length;
				if(min>1)
				{
					min=scope.hostsbck.values[1];
				}
				else
				{
					min=0;
				}
				var paragraph = element;
				$(paragraph).datepicker({dateFormat: "yy-mm-dd",minDate:min,maxDate:0,showButtonPanel: true,
					onSelect: function(date){
						var d = new Date();
						d=d.toISOString().slice(0,10);
						if(d == date)
						{
							scope.hostsbck.hostfile="Current";
						}
						else
						{
							scope.hostsbck.hostfile=date;
						}
						scope.hostsUpdate();
					},
					beforeShowDay: function(d) {
          				    	// normalize the date for searching in array
					        var dmy = "";
						dmy += d.getFullYear() + "-";
						dmy += ("00" + (d.getMonth() + 1)).slice(-2) + "-";
						dmy += ("00" + d.getDate()).slice(-2);
						if ($.inArray(dmy, scope.barGraph.labels) >= 0) {
							return [true, ""];
					        }
					        else {
					                return [false, ""];
					        }
					} 
				});
			}
		});
	};
	
	return {
		restrict: 'A',
		link: linkFunction
	};
})
.factory('mgInformer', function(){
	var errorWatchers = [];
	return {
		addErrorWatcher: function(fn) {
			errorWatchers.push(fn);
		},
	  allInfos: [],
	  inform: function(type, msg) {
		this.allInfos.push({
		  message: msg,
		  type: 'alert-' + type
		});
		angular.forEach(errorWatchers, function(fn){
		  fn();
		});
	  },
	  remove: function(index) {
		this.allInfos.splice(index, 1);
	  },
	
	  removeAll: function() {
		this.allInfos.length = 0;
	  }
	};
})
.directive("mgAlert", function(mgInformer, $location, $anchorScroll, $timeout) {
	return {
		restrict: 'E',
		scope: true,
		template: 	'<div ng-repeat="inform in informs">' +
					'<div ng-class="inform.type" class="alert alert-dismissable fade in informer">' +
					'<button type="button" ng-click="click($index)" class="close">&times;</button>' +
					'<div class="valignCenterWrapper">' +
					'<div class="valignCenter">' +
					'{{inform.message}}' +
					'</div></div></div></div></div>',
		controller: function($scope) {
			$scope.lastTimeout = {};
			mgInformer.addErrorWatcher(function() {
				$location.hash("top");
				$anchorScroll();
				if ($scope.lastTimeout != {}) $timeout.cancel($scope.lastTimeout);
				$scope.lastTimeout = $timeout(function(){
					$scope.lastTimeout = {};
					mgInformer.removeAll();
				}, 5000);
			});
			$scope.informs = mgInformer.allInfos;
			$scope.click = function(index) {
				mgInformer.remove(index);
			};
		}
	}
});
