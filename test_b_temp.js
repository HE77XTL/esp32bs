// 测试JavaScript文件
function testFunction() {
    console.log("Hello from b_temp.js");
    return "Test successful";
}

// 测试对象
const testObject = {
    name: "Test Object",
    value: 42,
    method: function() {
        return this.value * 2;
    }
};

// 测试数组
const testArray = [1, 2, 3, 4, 5];

// 导出测试函数
module.exports = {
    testFunction,
    testObject,
    testArray
};
