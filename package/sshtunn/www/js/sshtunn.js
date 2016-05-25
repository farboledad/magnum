plugInModule.controller('sshTunnCtrl',function($scope,$http,$log){
	
	$scope.tunnel={user:"",serIP:"",serPort:"",monPort:"",start:false};
	$scope.master={val:"",val1:"",val2:"",val3:"",val4:""};

	$scope.objCopy = function(){
		$scope.master.val = $scope.tunnel.user;
                $scope.master.val1 = $scope.tunnel.serIP;
                $scope.master.val2 = $scope.tunnel.serPort;
                $scope.master.val3 = $scope.tunnel.monPort;
		$scope.master.val4 = $scope.tunnel.start;
	};
	
	$scope.emptyObj = function(){
                if($scope.tunnel.user == "empty") $scope.tunnel.user = "";
                if($scope.tunnel.serIP == "empty") $scope.tunnel.serIP = "";
                if($scope.tunnel.serPort == "empty") $scope.tunnel.serPort = "";
                if($scope.tunnel.monPort == "empty") $scope.tunnel.monPort = "";
        };
	
	$scope.getConfig = function(){
        $http.get('/api/libcg/simple_json_get_tunnel_backend').success(function(data){
					$scope.tunnel = data;
					$scope.emptyObj();
					$scope.objCopy();
		});
	};
	
	$scope.certGenerate = function(){
                $scope.certificate="Reading...";
                $http.get('/api/libcg/simple_json_get_cert_backend').success(function(data){
                        $scope.certificate = data;                                                            
                });
        };
	
	$scope.getConfig();
	$scope.certGenerate();
		
	$scope.submitConfig = function(){
		$http.post('/api/libcg/simple_json_post_tunnel_backend',$scope.tunnel).success(function(data){
			$scope.getConfig();
		});
		$scope.objCopy();
	};
	
	$scope.onTunnelFormReset = function(){
		$scope.getConfig();
	};
		
	$scope.urlValidate = function(){
		var isValid = false;
		var ipformat = /^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$/;
		var urlformat = /[-a-zA-Z0-9@:%_\+.~#?&//=]{2,256}\.[a-z]{2,4}\b(\/[-a-zA-Z0-9@:%_\+.~#?&//=]*)?/gi;
		if(this.tunnel.serIP)
		{
			if(this.tunnel.serIP.match(ipformat) || this.tunnel.serIP.match(urlformat)) isValid = true;  
			else 
			{
				$log.log("Invalid IP address or URL");  
			}
		}
		this.configForm.URL.$setValidity('ip',isValid);
		return isValid;
	};
	$scope.serverPortValidate = function(){
                var isValid = false;
		var min = 1;
		var max = 65534;

                if(this.tunnel.serPort)
		{
			if(this.tunnel.serPort >= min && this.tunnel.serPort <= max)
			{
				isValid = true;
				this.tunnel.monPort = String(Number(this.tunnel.serPort) + 1);
			}
		}
		this.configForm.serPort.$setValidity('serPort',isValid);
                return isValid;
        };
        $scope.userValidate = function(){
        	var isValid = false;
        	if(this.tunnel.user) isValid = true;
        	this.configForm.user.$setValidity('user',isValid);
        	return isValid;
        };
        $scope.objValEqual = function(){
        	var eq = false;
        	if($scope.tunnel.user == $scope.master.val && $scope.tunnel.serIP == $scope.master.val1 && $scope.tunnel.serPort == $scope.master.val2 && $scope.tunnel.monPort == $scope.master.val3 && $scope.tunnel.start == $scope.master.val4)
        	{ eq = true;}
        	return eq;
        };
        $scope.tunnelModifiedValidate = function(){
        	var isValid = false;
        	if(!$scope.objValEqual())isValid = true;
        	return isValid;
        };
	$scope.validate = function(){
                var isValid = false;
		if($scope.urlValidate() && $scope.serverPortValidate() && $scope.userValidate() && $scope.tunnelModifiedValidate()) isValid=true;
                return isValid;
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
